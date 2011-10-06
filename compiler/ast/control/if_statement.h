// chime: if_statement.h

#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include "Conditional.h"

namespace ast
{
    class IfStatement : public Conditional
    {
    public:
        IfStatement(chime::parser& parser, ast::node* body);
        
        virtual std::string nodeName(void) const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        llvm::Value* codegen(chime::code_generator& generator);
    };
}
#endif // IF_STATEMENT_H
