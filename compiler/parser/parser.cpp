#include <string>
#include <vector>

#include "parser.h"

#define LOG(x) do { printf(x); fflush(stdout); } while (0);

namespace chime
{
    parser::parser(chime::lexer* lexer)
    {
        _lexer = lexer;
    }
    
    parser::~parser()
    {
    }
    
    
#pragma mark *** Token Traversal ***
    token* parser::next_token(void)
    {
        return this->next_token(NULL);
    }
    token* parser::next_token(const char* expected)
    {
        token* t = _lexer->next_token();
        
        if (t == NULL)
        {
            LOG("[Parser] Reached the end of the file unexpectedly\n")
            return NULL;
        }
        
        if ((expected != NULL) && (!t->equal_to(expected)))
        {
            printf("[Parser] Expected '%s' but got '%s'\n", expected, t->value.c_str());
        }
        
        return t;
    }
    bool parser::advance_token_if_equals(const char* expected)
    {
        token* t;
        
        t = this->look_ahead();
        if ((t != NULL) && t->equal_to(expected))
        {
            t = this->next_token(expected);
            delete t;
            return true;
        }
        
        return false;
    }
    std::string parser::next_token_value(void)
    {
        return this->next_token_value(NULL);
    }
    std::string parser::next_token_value(const char* expected)
    {
        token *t = this->next_token(expected);
        std::string v = t->value;
        
        delete t;
        
        return v;
    }
    token* parser::look_ahead(void)
    {
        return this->look_ahead(1);
    }
    token* parser::look_ahead(int distance)
    {
        return _lexer->look_ahead(distance);
    }
}
