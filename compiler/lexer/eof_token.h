#ifndef EOF_TOKEN_H
#define EOF_TOKEN_H

#include "compiler/lexer/token.h"

namespace chime
{
    class eof_token : public token
    {
    public:
        virtual bool equal_to(const char*);
        
        virtual bool empty(void) const;
        virtual bool isString(void) const;
        virtual bool isInteger(void) const;
        virtual bool isFloatingPoint(void) const;
        virtual bool isBoolean(void) const;
        virtual bool isModifier(void) const;
        virtual bool isStructural(void) const;
        virtual bool isConditional(void) const;
        virtual bool isLoop(void) const;
        virtual bool isControl(void) const;
        virtual bool isIdentifier(void) const;
        virtual bool isType(void) const;
        virtual bool isPunctuation(void) const;
        virtual bool isReserved(void) const;
        virtual bool is_block_start(void);
        virtual bool isEnding(void) const;
        virtual bool isExceptionRelated(void) const;
        virtual int  precedence(void) const;
        
        virtual long integer_value(void);
        virtual std::string string_value(void);
    };
}

#endif // EOF_TOKEN_H
