// chime: catch.cpp

#include "ast/control/catch.h"
#include "parser/parser.h"

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
            _exceptionVariable = new ast::entity_reference(&parser);
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
    
    std::string Catch::nodeName(void)
    {
        return std::string("catch");
    }
}
