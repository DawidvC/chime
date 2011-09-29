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
        bool isPlainString() const;
        bool isInterpolatedStringStart() const;
        bool isInterpolatedStringEnd() const;
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
        virtual bool isEnding(void) const;
        virtual bool isBlockEnding() const;
        virtual bool isStatementEnding() const;
        virtual bool isExceptionRelated(void) const;
        virtual int  precedence(void) const;
        
        virtual long integerValue(void);
        virtual std::string stringValue(void);
    };
}

#endif // EOF_TOKEN_H
