#include <string>

#ifndef _TOKEN_H_
#define _TOKEN_H_

namespace chime
{
    class token
    {
    public:
        std::string value;
        
        token();
        virtual ~token();
        
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
        virtual int precedence(void);
        
        virtual long integer_value(void);
        virtual std::string string_value(void);
    };
}

#endif /* _TOKEN_H_ */
