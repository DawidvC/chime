// chime: Variable.h

#ifndef VARIABLE_H
#define VARIABLE_H

#include "compiler/ast/node.h"
#include "compiler/ast/operators/AssignmentOperator.h"

namespace chime
{
    class Variable : public Node
    {
    public:
        static Node* parse(parser& parser, bool allowAssignment=true);
        static Node* parseAssignment(parser& parser, Variable* variable);
        
    public:
        Variable(const std::string& identifier);
        
        std::string stringRepresentation(int depth=0) const;
        
        std::string getIdentifier() const;
        std::string identifier() const;
        
        bool        isDefined() const;
        void        setDefined(bool value);
        
        bool         isVariable() const;
        bool         requiresRelease() const;
        virtual bool requiresCapture() const;
        
        virtual ast::AssignmentOperator* createAssignment() = 0;
        virtual Variable*                createClosedVersion();
        
        virtual llvm::Value* codegenReference(CodeGenContext& generator);
    
    protected:
        std::string _identifier;
        bool        _defined;
    };
    
    typedef shared_ptr<Variable> VariableRef;
}

#endif // VARIABLE_H
