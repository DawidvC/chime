#include "ScopedNode.h"

namespace ast
{
    ScopedNode::ScopedNode()
    {
        _parent = NULL;
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
        ScopedNode* node;
        
        node = this->getParent();
        
        while (node)
        {
            if (node->definedIdentifier(identifier))
            {
                Variable* v;
                
                v = node->createVariable(identifier);
                v = this->transformVariable(v);
                
                return v;
            }
            
            node = node->getParent();
        }
        
        _variableNames.push_back(identifier); // track it here
        return this->createVariable(identifier);
    }
    
    llvm::Value* ScopedNode::getValueForIdentifier(const std::string& identifier)
    {
        return _scopedValues[identifier];
    }
    
    void ScopedNode::setValueForIdentifier(const std::string& identifier, llvm::Value* value)
    {
        _scopedValues[identifier] = value;
    }
}
