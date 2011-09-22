#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>
#include <tr1/memory>

using std::tr1::shared_ptr;

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
        
        virtual bool empty(void) const;
        bool isString() const;
        virtual bool isPlainString() const;
        virtual bool isInterpolatedStringStart() const;
        virtual bool isInterpolatedStringMiddle() const;
        virtual bool isInterpolatedStringEnd() const;
        bool         isNumber(bool allowFloat=true) const;
        virtual bool is_integer(void);
        virtual bool isInteger(void) const;
        virtual bool is_floating_point(void);
        virtual bool isFloatingPoint(void) const;
        virtual bool is_boolean(void);
        virtual bool isBoolean(void) const;
        virtual bool isInheritanceRelated(void) const;
        virtual bool is_modifier(void);
        virtual bool isModifier(void) const;
        virtual bool isStructural(void) const;
        virtual bool is_conditional(void);
        virtual bool isConditional(void) const;
        virtual bool is_loop(void);
        virtual bool isLoop(void) const;
        virtual bool is_control(void);
        virtual bool isControl(void) const;
        bool         is_literal(void);
        bool         isLiteral(void) const;
        virtual bool is_identifier(void);
        virtual bool isIdentifier(void) const;
        virtual bool is_type(void);
        virtual bool isType(void) const;
        virtual bool is_punctuation(void);
        virtual bool isPunctuation(void) const;
        virtual bool is_reserved(void);
        virtual bool isReserved(void) const;
        virtual bool is_ending(void) const;
        virtual bool isEnding() const;
        virtual bool isExceptionRelated(void) const;
        virtual int precedence(void) const;
        
        virtual long integer_value(void);
        virtual std::string string_value(void);
    
    protected:
        std::string _value;
    };
    
    typedef shared_ptr<chime::token> TokenRef;
}

#endif /* _TOKEN_H_ */
