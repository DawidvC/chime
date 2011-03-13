#include "eof_token.h"

namespace chime
{
    bool eof_token::equal_to(const char* s)
    {
        return false;
    }
    
    bool eof_token::empty(void)
    {
        return true;
    }
    bool eof_token::is_string(void)
    {
        return false;
    }
    bool eof_token::is_number(void)
    {
        return false;
    }
    bool eof_token::is_integer(void)
    {
        return false;
    }
    bool eof_token::is_floating_point(void)
    {
        return false;
    }
    bool eof_token::is_boolean(void)
    {
        return false;
    }
    bool eof_token::is_modifier(void)
    {
        return false;
    }
    bool eof_token::is_structural(void)
    {
        return false;
    }
    bool eof_token::is_conditional(void)
    {
        return false;
    }
    bool eof_token::is_loop(void)
    {
        return false;
    }
    bool eof_token::is_control(void)
    {
        return false;
    }
    bool eof_token::is_literal(void)
    {
        return false;
    }
    bool eof_token::is_identifier(void)
    {
        return false;
    }
    bool eof_token::is_entity(void)
    {
        return false;
    }
    bool eof_token::is_punctuation(void)
    {
        return true;
    }
    bool eof_token::is_reserved(void)
    {
        return false;
    }
    bool eof_token::is_block_start(void)
    {
        return false;
    }
    int eof_token::precedence(void)
    {
        return -1;
    }
    
    long eof_token::integer_value(void)
    {
        return 0;
    }
    std::string eof_token::string_value(void)
    {
        return std::string("EOF");
    }
}
