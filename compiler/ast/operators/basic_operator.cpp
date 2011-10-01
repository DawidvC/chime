#include "basic_operator.h"

namespace ast
{
    basic_operator::basic_operator()
    {
    }
    
    basic_operator::~basic_operator()
    {
    }
    
    std::string basic_operator::identifier() const
    {
        return this->getIdentifier();
    }
    void basic_operator::identifier(std::string s)
    {
        this->setIdentifier(s);
    }
    
    std::string basic_operator::getIdentifier() const
    {
        return _identifier;
    }
    void basic_operator::setIdentifier(const std::string& s)
    {
        _identifier = s;
    }
}
