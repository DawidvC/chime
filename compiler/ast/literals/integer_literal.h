// chime: integer_literal.h

#ifndef INTEGER_LITERAL_H
#define INTEGER_LITERAL_H

#include "Literal.h"

namespace chime
{
    class IntegerLiteral : public ast::Literal
    {
    public:
        IntegerLiteral();
        
        virtual std::string nodeName(void) const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        long getValue() const;
        void setValue(long v);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
        
    private:
        long _value;
    };
}

#endif // INTEGER_LITERAL_H
