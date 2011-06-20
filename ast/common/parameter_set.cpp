// chime: parameter_set.cpp

#include "parameter_set.h"
#include "parser/parser.h"
#include "ast/common/method_parameter.h"

namespace ast
{
    ParameterSet::ParameterSet(chime::parser& parser)
    {
        parser.next_token_value("(");
        
        while (true)
        {
            chime::token* t;
            
            t = parser.look_ahead();
            
            if (t->empty() || t->equal_to(")"))
                break;
            
            this->addChild(new method_parameter(&parser));
            
            // take care of the commas
            if (parser.look_ahead()->equal_to(","))
                parser.next_token_value(",");
        }
        
        parser.next_token_value(")");
    }
    
    ParameterSet::~ParameterSet()
    {
    }
    
    std::string ParameterSet::nodeName(void) const
    {
        return std::string("parameter set");
    }
}
