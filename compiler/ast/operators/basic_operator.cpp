#include "basic_operator.h"

namespace ast
{
    basic_operator::basic_operator()
    {
    }
    
    std::string basic_operator::identifier() const
    {
        return _identifier;
    }
    void basic_operator::identifier(std::string s)
    {
        _identifier = s;
    }
}
