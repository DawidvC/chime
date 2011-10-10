// chime: Variable.h

#ifndef VARIABLE_H
#define VARIABLE_H

#include "compiler/ast/node.h"
#include "compiler/ast/operators/AssignmentOperator.h"

namespace ast
{
    class Variable : public node
    {
    public:
        static Node* parse(chime::parser& parser, bool allowAssignment=true);
        static Node* parseAssignment(chime::parser& parser, Variable* variable);
        
    public:
        Variable(const std::string& identifier);
        
        std::string stringRepresentation(int depth=0) const;
        
        std::string getIdentifier() const;
        
        virtual AssignmentOperator* createAssignment() = 0;
        
        virtual llvm::Value* codegenReference(chime::code_generator& generator);
    
    protected:
        std::string _identifier;
    };
    
    typedef std::tr1::shared_ptr<Variable> VariableRef;
}

#endif // VARIABLE_H
