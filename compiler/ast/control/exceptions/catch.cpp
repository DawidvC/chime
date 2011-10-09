// chime: catch.cpp

#include "catch.h"

namespace ast
{
    Catch::Catch(chime::parser& parser)
    {
        chime::token* t;
        
        _exceptionType     = NULL;
        _exceptionVariable = NULL;
        
        parser.next_token("catch");
        
        parser.next_token("(");
        
        t = parser.look_ahead();
        if (t->isType())
        {
            _exceptionType = new ast::type_reference(&parser);
            
            t = parser.look_ahead();
        }
        
        if (t->isIdentifier())
        {
            // do not allow assignment
            _exceptionVariable = static_cast<ast::Variable*>(ast::Variable::parse(parser, false));
        }
        
        parser.next_token(")");
        
        this->parseBody(parser);
    }
    
    Catch::~Catch()
    {
        if (_exceptionType)
            delete _exceptionType;
        
        if (_exceptionVariable)
            delete _exceptionVariable;
    }
    
    std::string Catch::nodeName(void) const
    {
        return std::string("catch");
    }
}
