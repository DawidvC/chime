// chime: lexer.h

#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include <string>
#include <vector>

namespace chime
{
    class lexer
    {
    public:
        lexer();
        virtual ~lexer();
        
        token* next_token(void);
        token* look_ahead(unsigned int);
        
        void raise_error(std::string);
        void invalid_token(void);
        
        bool ignore_new_lines() const;
        void ignore_new_lines(bool i);
        
        int  current_line(void) const;
        
    protected:
        bool                 _ignore_new_lines;
        int                  _current_line;
        std::vector<token*>* token_buffer;
        
        token* extract_next_token(void);
        
        virtual bool is_finished(void) = 0;
        virtual char next_char(void)   = 0;
        virtual char peek(void)        = 0;
        
        char escapeCodeToChar(char c);
        
    private:
        char _peeked_char;
        bool _inString;
        bool _interpolatingString;
    };
    
    typedef shared_ptr<lexer> LexerRef;
}

#endif // LEXER_H
