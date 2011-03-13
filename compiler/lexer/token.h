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
        ~token();
        
        bool equal_to(const char*);
        
        bool empty(void);
        bool is_string(void);
        bool is_number(void);
        bool is_integer(void);
        bool is_floating_point(void);
        bool is_boolean(void);
        bool is_modifier(void);
        bool is_structural(void);
        bool is_conditional(void);
        bool is_loop(void);
        bool is_control(void);
        bool is_literal(void);
        bool is_identifier(void);
        bool is_entity(void);
        bool is_punctuation(void);
        bool is_reserved(void);
        bool is_block_start(void);
        int precedence(void);
        
        long integer_value(void);
        std::string string_value(void);
    };
}

#endif /* _TOKEN_H_ */
