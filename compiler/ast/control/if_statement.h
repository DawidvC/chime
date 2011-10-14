// chime: if_statement.h

#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include "Conditional.h"

namespace chime
{
    class IfStatement : public Conditional
    {
    public:
        virtual std::string nodeName() const;
        
        llvm::Value* codegen(code_generator& generator);
    };
}
#endif // IF_STATEMENT_H
