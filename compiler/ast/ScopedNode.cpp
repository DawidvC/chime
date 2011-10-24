#include "ScopedNode.h"

#include <sstream>

namespace ast
{
    ScopedNode::ScopedNode()
    {
        _parent        = NULL;
        _selfObjectPtr = NULL;
        
        _anonymousFunctionCount = 1;
    }
    
    ScopedNode* ScopedNode::getParent() const
    {
        return _parent;
    }
    
    void ScopedNode::setParent(ScopedNode* parent)
    {
        _parent = parent;
    }
    
    std::vector<std::string> ScopedNode::getContainedVariableNames()
    {
        return std::vector<std::string>();
    }
    
    bool ScopedNode::definedIdentifier(const std::string& identifier)
    {
        return std::find(_variableNames.begin(), _variableNames.end(), identifier) != _variableNames.end();
    }
    
    bool ScopedNode::capturedIdentifier(const std::string& identifier)
    {
        return _capturedVariables.count(identifier) == 1; 
    }
    
    Variable* ScopedNode::createVariable(const std::string& identifier)
    {
        return NULL;
    }
    
    Variable* ScopedNode::transformVariable(Variable* variable)
    {
        return variable;
    }
    
    void ScopedNode::capturedVariable(Variable* variable)
    {
        _capturedVariables[variable->getIdentifier()] = variable;
    }
    
    Variable* ScopedNode::variableForIdentifier(const std::string& identifier)
    {
        ScopedNode*              node;
        std::vector<ScopedNode*> nodesPassedOver;
        
        node = this->getParent();
        
        while (node)
        {
            if (node->definedIdentifier(identifier))
            {
                Variable* v;
                
                // allow the node that actually defined the identifier to make
                // the variable node
                v = node->createVariable(identifier);
                
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
            
            node = node->getParent();
        }
        
        _variableNames.push_back(identifier); // track it here
        return this->createVariable(identifier);
    }
    
    bool ScopedNode::allowsStructuralElements() const
    {
        return false; // most scopes do not allow this
    }
    
    llvm::Value* ScopedNode::getValueForIdentifier(const std::string& identifier)
    {
        llvm::Value* value;
        
        value = _scopedValues[identifier];
        
        if (value || !this->getParent())
            return value;
        
        return this->getParent()->getValueForIdentifier(identifier);
    }
    
    void ScopedNode::setValueForIdentifier(const std::string& identifier, llvm::Value* value)
    {
        _scopedValues[identifier] = value;
    }
    
    llvm::Value* ScopedNode::getSelfObjectPtr() const
    {
        return _selfObjectPtr;
    }
    
    void ScopedNode::setSelfObjectPtr(llvm::Value* value)
    {
        _selfObjectPtr = value;
    }
    
    std::string ScopedNode::getAnonymousFunctionName()
    {
        std::stringstream s;
        
        s << this->getIdentifier();
        s << "_anonymous_";
        s << _anonymousFunctionCount;
        
        _anonymousFunctionCount += 1;
        
        return s.str();
    }
    
    llvm::BasicBlock* ScopedNode::getStartBlock() const
    {
        return NULL;
    }
    
    llvm::BasicBlock* ScopedNode::getEndBlock() const
    {
        return NULL;
    }
}
