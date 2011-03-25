#include <string>

#ifndef _TOKEN_H_
#define _TOKEN_H_

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
        std::string value(void) const;
        const char* c_str(void) const;
        
        virtual bool equal_to(const char*);
        
        virtual bool empty(void);
        virtual bool is_string(void);
        virtual bool is_number(void);
        virtual bool is_integer(void);
        virtual bool is_floating_point(void);
        virtual bool is_boolean(void);
        virtual bool is_modifier(void);
        virtual bool is_structural(void);
        virtual bool is_conditional(void);
        virtual bool is_loop(void);
        virtual bool is_control(void);
        virtual bool is_literal(void);
        virtual bool is_identifier(void);
        virtual bool is_type(void);
        virtual bool is_punctuation(void);
        virtual bool is_reserved(void);
        virtual bool is_block_start(void);
        virtual bool is_ending(void) const;
        virtual int precedence(void);
        
        virtual long integer_value(void);
        virtual std::string string_value(void);
    
    protected:
        std::string _value;
    };
}

#endif /* _TOKEN_H_ */
