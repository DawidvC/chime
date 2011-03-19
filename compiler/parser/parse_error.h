#include <string>

#ifndef PARSE_ERROR
#define PARSE_ERROR

namespace chime
{
    class parse_error
    {
    public:
        parse_error(const char* format, ...);
        virtual ~parse_error();
        
        std::string message() const;
        
    protected:
        std::string* _message;
    };
}

#endif PARSE_ERROR
