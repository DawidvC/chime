#include <cstdio>
#include <string>

#include "file_lexer.h"

namespace chime
{
    filelexer::filelexer(const char *path)
    {
        input_file.open(path, std::ios::in);
    }

    filelexer::~filelexer()
    {
        this->close();
    }
    
    void filelexer::close()
    {
        input_file.close();
    }
    
    char filelexer::next_char()
    {
        if (!this->is_finished())
            return input_file.get();
            
        return 0;
    }
    
    char filelexer::peek()
    {
        if (!input_file)
            return 0;
            
        return input_file.peek();
    }
    
    bool filelexer::is_finished()
    {
        if (!input_file)
            return true;
            
        return !input_file.good();
    }
}
