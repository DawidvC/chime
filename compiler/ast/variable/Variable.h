// chime: Variable.h

#ifndef VARIABLE_H
#define VARIABLE_H

#include "compiler/ast/node.h"

namespace ast
{
    class Variable : public node
    {
    public:
        static node* parse(chime::parser& parser);
        
    public:
        Variable(const std::string& identifier);
        
        std::string  getIdentifier() const;
        
    protected:
        std::string _identifier;
    };
    
    typedef std::tr1::shared_ptr<Variable> VariableRef;
}

#endif // VARIABLE_H
