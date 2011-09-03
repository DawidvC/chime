// chime: ScopedNode.h

#ifndef SCOPED_NODE_H
#define SCOPED_NODE_H

#include "compiler/ast/node.h"
#include "compiler/ast/variable/Variable.h"

namespace ast
{
    class ScopedNode : public node
    {
    public:
        ScopedNode();
        
        ScopedNode* getParent() const;
        void        setParent(ScopedNode* parent);
        
        std::vector<std::string> getContainedVariableNames();
        bool                     definedIdentifier(const std::string& identifier);
        
        virtual Variable* createVariable(const std::string& identifier);
        virtual void      definedInParent(Variable* variable);
        Variable*         variableForIdentifier(const std::string& identifier);
        
    protected:
        std::vector<std::string> _variableNames;
        ScopedNode*              _parent;
    };
    
    typedef std::tr1::shared_ptr<ScopedNode> ScopedNodeRef;
}

#endif // SCOPED_NODE_H
