#include "eof_token.h"

namespace chime
{
    bool eof_token::equal_to(const char* s)
    {
        return false;
    }
    
    bool eof_token::empty(void) const
    {
        return true;
    }
    
    bool eof_token::isPlainString() const
    {
        return false;
    }
    
    bool eof_token::isInterpolatedStringStart() const
    {
        return false;
    }
    
    bool eof_token::isInterpolatedStringEnd() const
    {
        return false;
    }
    
    bool eof_token::isInteger(void) const
    {
        return false;
    }
    bool eof_token::isFloatingPoint(void) const
    {
        return false;
    }
    bool eof_token::isBoolean(void) const
    {
        return false;
    }
    bool eof_token::isModifier(void) const
    {
        return false;
    }
    bool eof_token::isStructural(void) const
    {
        return false;
    }
    bool eof_token::isConditional(void) const
    {
        return false;
    }
    bool eof_token::isLoop(void) const
    {
        return false;
    }
    bool eof_token::isControl(void) const
    {
        return false;
    }
    bool eof_token::isAssignment() const
    {
        return false;
    }
    
    bool eof_token::isIdentifier(void) const
    {
        return false;
    }
    
    bool eof_token::isType(void) const
    {
        return false;
    }
    
    bool eof_token::isPunctuation(void) const
    {
        return true;
    }
    
    bool eof_token::isReserved(void) const
    {
        return false;
    }
    bool eof_token::isEnding(void) const
    {
        return true;
    }
    bool eof_token::isBlockEnding() const
    {
        return true;
    }
    bool eof_token::isStatementEnding() const
    {
        return true;
    }
    
    bool eof_token::isExceptionRelated(void) const
    {
        return false;
    }
    
    int eof_token::precedence(void) const
    {
        return -1;
    }
    
    long eof_token::integerValue(void)
    {
        return 0;
    }
    std::string eof_token::stringValue(void)
    {
        return std::string("EOF");
    }
}
