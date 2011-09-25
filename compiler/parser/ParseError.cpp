#include "ParseError.h"

#include <stdarg.h>

namespace chime
{
    ParseError::ParseError(const char* format, ...)
    {
        char buffer[1024];
        
        va_list list;
        
        va_start(list, format);
        
        vsnprintf(buffer, 1024, format, list);
        
        va_end(list);
        
        _message = new std::string(buffer);
        
        this->line(-1);
    }
    
    ParseError::~ParseError()
    {
        delete _message;
    }
    
    std::string ParseError::message() const
    {
        return *_message;
    }
    
    int ParseError::line(void) const
    {
        return _line;
    }
    
    void ParseError::line(int l)
    {
        _line = l;
    }
}
