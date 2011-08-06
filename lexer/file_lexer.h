#ifndef FILE_LEXER_H
#define FILE_LEXER_H

#include "lexer.h"

#include <fstream>
#include <tr1/memory>

namespace chime
{
    class filelexer : public lexer
    {
    public:
        filelexer(const char *path);
        ~filelexer();
        
    protected:
        std::ifstream input_file;
        
        bool is_finished(void);
        void close(void);
        char next_char(void);
        char peek(void);
    };
    
    typedef std::tr1::shared_ptr<filelexer> FileLexerRef;
}

#endif // FILE_LEXER_H
