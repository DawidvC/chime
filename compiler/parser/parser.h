#include "../lexer/lexer.h"

#ifndef PARSER
#define PARSER

namespace chime
{
    class parser
    {
    public:
        parser(chime::lexer* lexer);
        ~parser();
        
        token* next_token(void);
        token* next_token(const char* expected);
        bool advance_token_if_equals(const char* expected);
        std::string next_token_value(void);
        std::string next_token_value(const char* expected);
        token* look_ahead(void);
        token* look_ahead(int);
        
    protected:
        chime::lexer *_lexer;
    };
}

#endif PARSER
