#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>
#include <tr1/memory>

namespace chime
{
    class token
    {
    public:
        token();
        virtual ~token();
        
        void clear_value(void);
        void set_value(const char c);
        void set_value(const char* s);
        void append(const char c);
        void append(const std::string& str);
        std::string value(void) const;
        const char* c_str(void) const;
        
        virtual bool equal_to(const char*);
        
        bool         operator==(const char* cString);
        bool         operator!=(const char* cString);
        
        virtual bool empty(void) const;
        bool isString() const;
        virtual bool isPlainString() const;
        virtual bool isInterpolatedStringStart() const;
        virtual bool isInterpolatedStringMiddle() const;
        virtual bool isInterpolatedStringEnd() const;
        bool         isNumber(bool allowFloat=true) const;
        virtual bool isInteger(void) const;
        virtual bool isFloatingPoint(void) const;
        virtual bool isBoolean(void) const;
        virtual bool isInheritanceRelated(void) const;
        virtual bool isModifier(void) const;
        virtual bool isStructural(void) const;
        virtual bool isConditional(void) const;
        virtual bool isLoop(void) const;
        virtual bool isControl(void) const;
        bool         isLiteral(void) const;
        virtual bool isIdentifier(void) const;
        virtual bool isType(void) const;
        virtual bool isPunctuation(void) const;
        virtual bool isReserved(void) const;
        virtual bool isEnding() const;
        virtual bool isBlockEnding() const;
        virtual bool isStatementEnding() const;
        virtual bool isExceptionRelated(void) const;
        virtual int precedence(void) const;
        
        virtual long integerValue(void);
        virtual std::string stringValue(void);
    
    protected:
        std::string _value;
    };
    
    typedef chime::token Token;
    typedef std::tr1::shared_ptr<chime::token> TokenRef;
}

#endif /* _TOKEN_H_ */
