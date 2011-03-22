#include "token.h"

#ifndef _EOF_TOKEN_H_
#define _EOF_TOKEN_H_

namespace chime
{
    class eof_token : public token
    {
    public:
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
        virtual bool is_entity(void);
        virtual bool is_punctuation(void);
        virtual bool is_reserved(void);
        virtual bool is_block_start(void);
        virtual bool is_ending(void) const;
        virtual int precedence(void);
        
        virtual long integer_value(void);
        virtual std::string string_value(void);
    };
}

#endif /* _EOF_TOKEN_H_ */
