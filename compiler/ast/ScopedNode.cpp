#include "ScopedNode.h"

#include <sstream>

// #define PRINT_TRACKING

namespace chime
{
    ScopedNode::ScopedNode()
    {
        _parent         = NULL;
        _implementation = NULL;
        _selfObjectPtr  = NULL;
        _classObjectPtr = NULL;
        
        _anonymousFunctionCount = 1;
        
        _startBlock = NULL;
        _endBlock   = NULL;
    }
    
    ScopedNode* ScopedNode::parent() const
    {
        return _parent;
    }
    
    void ScopedNode::setParent(ScopedNode* parent)
    {
        assert(parent != this);
        
        _parent = parent;
    }
    
    ScopedNode* ScopedNode::ancestorThatDefinesSelf() const
    {
        for (ScopedNode* node = this->parent(); node != NULL; node = node->parent())
        {
            if (node->selfObjectPtr())
            {
                return node;
            }
        }
        
        return NULL;
    }
    
    ScopedNode* ScopedNode::ancestorThatDefinesClassObject() const
    {
        for (ScopedNode* node = this->parent(); node != NULL; node = node->parent())
        {
            if (node->classObjectPtr())
            {
                return node;
            }
        }
        
        return NULL;
    }
    
    ScopedNode* ScopedNode::enclosingImplementation() const
    {
        return _implementation;
    }
    
    void ScopedNode::setEnclosingImplementation(ScopedNode* implementation)
    {
        assert(implementation != this);
        
        _implementation = implementation;
    }
    
    bool ScopedNode::definedIdentifier(const std::string& identifier) const
    {
        return std::find(_variableNames.begin(), _variableNames.end(), identifier) != _variableNames.end();
    }
    
    void ScopedNode::defineIdentifier(const std::string& identifier)
    {
        _variableNames.push_back(identifier); // track it here
    }
    
    bool ScopedNode::capturedIdentifier(const std::string& identifier)
    {
        return _capturedVariables.count(identifier) == 1; 
    }
    
    chime::Variable* ScopedNode::createVariable(const std::string& identifier)
    {
        return NULL;
    }
    
    chime::Variable* ScopedNode::transformVariable(chime::Variable* variable)
    {
        return variable;
    }
    
    void ScopedNode::capturedVariable(chime::Variable* variable)
    {
        _capturedVariables[variable->getIdentifier()] = variable;
    }
    
    chime::Variable* ScopedNode::variableForIdentifier(const std::string& identifier)
    {
        ScopedNode*              node;
        chime::Variable*         v;
        std::vector<ScopedNode*> nodesPassedOver;
        
        // check the simple case, that we've defined it at this scope level
        if (this->definedIdentifier(identifier))
        {
            v = this->createVariable(identifier);
            v->setDefined(true);
            
            return v;
        }
        
        node = this->parent();
        
        while (node)
        {
            if (node->definedIdentifier(identifier))
            {
                // allow the node that actually defined the identifier to make
                // the variable node
                v = node->createVariable(identifier);
                v->setDefined(true);
                
                // This is kind of a hack.  It gives all of the inner nodes a chance
                // to transform the variable.  This is specifically so that closures
                // can be notified of inner variables they need to close over
                for (std::vector<ScopedNode*>::iterator it = nodesPassedOver.begin(); it != nodesPassedOver.end(); ++it)
                {
                    v = (*it)->transformVariable(v);
                }
                
                // now, allow the node that's making the reference to transform it, 
                // if needed
                v = this->transformVariable(v);
                
                return v;
            }
            
            nodesPassedOver.push_back(node);
            
            node = node->parent();
        }
        
        this->defineIdentifier(identifier);
        
        // If node is null at this point, this variable has yet to be defined.
        
        return this->createVariable(identifier);
    }
    
    chime::SelfLiteral* ScopedNode::createSelf()
    {
        return NULL;
    }
    
    chime::SelfLiteral* ScopedNode::getSelf()
    {
        ScopedNode*         node;
        chime::SelfLiteral* self;
        
        node = this;
        
        while (node && !self)
        {
            self = node->createSelf();
            
            node = node->parent();
        }
        
        return self;
    }
    
    bool ScopedNode::allowsStructuralElements() const
    {
        return false; // most scopes do not allow this
    }
    
    bool ScopedNode::isFunction() const
    {
        return false;
    }
    
    void ScopedNode::addLooseValue(llvm::Value* value)
    {
        // fprintf(stderr, "<%p> tracking %p\n", this, value);
        _looseValues.push_back(value);
    }
    
    void ScopedNode::removeLooseValue(llvm::Value* value)
    {
        std::vector<llvm::Value*>::iterator it;
        
        it = std::find(_looseValues.begin(), _looseValues.end(), value);
        if (it == _looseValues.end())
            return;
        
        // fprintf(stderr, "<%p> not tracking %p\n", this, value);
        _looseValues.erase(it);
    }
    
    llvm::Value* ScopedNode::getValueForIdentifier(const std::string& identifier)
    {
        if (_scopedValues.count(identifier) > 0)
            return _scopedValues[identifier];
        
        if (!this->parent())
            return NULL;
        
        return this->parent()->getValueForIdentifier(identifier);
    }
    
    void ScopedNode::setValueForIdentifier(const std::string& identifier, llvm::Value* value, bool shouldRelease)
    {
        _scopedValues[identifier] = value;
        
        if (!shouldRelease)
            _scopedValuesToSkip.push_back(identifier);
    }
    
    llvm::Value* ScopedNode::selfValue(chime::CodeGenContext& context)
    {
        return this->selfObjectPtr();
    }
    
    llvm::Value* ScopedNode::selfObjectPtr() const
    {
        return _selfObjectPtr;
    }
    
    void ScopedNode::setSelfObjectPtr(llvm::Value* value)
    {
        _selfObjectPtr = value;
    }
    
    llvm::Value* ScopedNode::classObjectPtr() const
    {
        return _classObjectPtr;
    }
    void ScopedNode::setClassObjectPtr(llvm::Value* value)
    {
        _classObjectPtr = value;
    }
    
    std::map<std::string, llvm::Value*> ScopedNode::scopedValues() const
    {
        return _scopedValues;
    }
    
    void ScopedNode::codegenScopeExit(chime::CodeGenContext& context, std::vector<std::string> identifiersToSkip)
    {
        std::map<std::string, llvm::Value*>::const_iterator it;
        
        if (context.builder()->GetInsertBlock()->getTerminator())
        {
            // fprintf(stderr, "<%s> terminated, no releases possible\n", this->getIdentifier().c_str());
            return;
        }
        
#ifdef PRINT_TRACKING
        fprintf(stderr, "scope exit: %p\n", this);
#endif
        
        for (it = _scopedValues.begin(); it != _scopedValues.end(); ++it)
        {
            // if this is in our list to skip, do not release
            if (std::find(identifiersToSkip.begin(), identifiersToSkip.end(), it->first) != identifiersToSkip.end())
            {
#ifdef PRINT_TRACKING
                fprintf(stderr, "<%s> instructed to skip %s\n", this->getIdentifier().c_str(), it->first.c_str());
#endif
                continue;
            }
            
            // if the value is in our skip list, we do not need to release
            if (std::find(_scopedValuesToSkip.begin(), _scopedValuesToSkip.end(), it->first) != _scopedValuesToSkip.end())
            {
#ifdef PRINT_TRACKING
                fprintf(stderr, "<%s> listed as a scoped value to skip %s\n", this->getIdentifier().c_str(), it->first.c_str());
#endif
                continue;
            }
            
#ifdef PRINT_TRACKING
            fprintf(stderr, "<%s> release: %s => %p (%p)\n", this->getIdentifier().c_str(), it->first.c_str(), it->second, this->selfObjectPtr());
#endif
            context.getRuntime()->callChimeObjectRelease(it->second);
        }
        
        std::vector<llvm::Value*>::const_iterator vit;
        for (vit = _looseValues.begin(); vit != _looseValues.end(); ++vit)
        {
#ifdef PRINT_TRACKING
            fprintf(stderr, "<%s> loose release: %p\n", this->getIdentifier().c_str(), (*vit));
#endif
            context.getRuntime()->callChimeObjectRelease((*vit));
        }
    }
    
    void ScopedNode::codegenFunctionExit(chime::CodeGenContext& context, std::vector<std::string> identifiersToSkip)
    {
        ScopedNode* node;
        
        node = this;
        
        while (node)
        {
            node->codegenScopeExit(context, identifiersToSkip);
            
            if (node->isFunction())
                break;
            
            node = node->parent();
        }
    }
    
    int ScopedNode::incrementAnonymousFunctionCount()
    {
        int count;
        
        count = _anonymousFunctionCount;
        
        _anonymousFunctionCount += 1;
        
        return count;
    }
    
    // these should be virtual too, so I can play the same tricks with the CodeBlock
    llvm::BasicBlock* ScopedNode::getStartBlock() const
    {
        if (_startBlock)
            return _startBlock;
        
        for (ScopedNode* node = this->parent(); node != NULL; node = node->parent())
        {
            if (node->getStartBlock())
                return node->getStartBlock();
        }
        
        return NULL;
    }
    
    void ScopedNode::setStartBlock(llvm::BasicBlock* block)
    {
        _startBlock = block;
    }
    
    llvm::BasicBlock* ScopedNode::getEndBlock() const
    {
        if (_endBlock)
            return _endBlock;
        
        for (ScopedNode* node = this->parent(); node != NULL; node = node->parent())
        {
            if (node->getEndBlock())
                return node->getEndBlock();
        }
        
        return NULL;
    }
    
    void ScopedNode::setEndBlock(llvm::BasicBlock* block)
    {
        _endBlock = block;
    }
}
