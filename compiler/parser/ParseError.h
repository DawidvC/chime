// chime: ParseError.h

#ifndef PARSE_ERROR_H
#define PARSE_ERROR_H

#include <string>
#include <tr1/memory>

namespace chime
{
    class ParseError
    {
    public:
        ParseError(const char* format, ...);
        virtual ~ParseError();
        
        std::string message() const;
        
        int         line(void) const;
        void        line(int l);
        
    protected:
        std::string* _message;
        int          _line;
    };
    
    typedef std::tr1::shared_ptr<chime::ParseError> ParseErrorRef;
}

#endif // PARSE_ERROR_H
