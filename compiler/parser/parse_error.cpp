#include "parse_error.h"
#include <stdarg.h>

namespace chime
{
    parse_error::parse_error(const char* format, ...)
    {
        char buffer[1024];
        
        va_list list;
        
        va_start(list, format);
        
        vsnprintf(buffer, 1024, format, list);
        
        va_end(list);
        
        _message = new std::string(buffer);
    }
    
    parse_error::~parse_error()
    {
        delete _message;
    }
    
    std::string parse_error::message() const
    {
        return *_message;
    }
}