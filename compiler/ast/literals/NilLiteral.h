// chime: NilLiteral.h

#ifndef NIL_LITERAL_H
#define NIL_LITERAL_H

#include "Literal.h"

namespace chime
{
    class NilLiteral : public ast::Literal
    {
    public:
        virtual std::string nodeName() const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        llvm::Value* codegen(CodeGenContext& context);
    };
}

#endif // NIL_LITERAL_H
