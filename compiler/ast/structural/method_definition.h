// chime: method_definition.h

#ifndef METHOD_DEFINITON_H
#define METHOD_DEFINITON_H

#include "compiler/ast/node.h"
#include "compiler/ast/common/parameter_set.h"
#include "compiler/ast/common/code_block.h"

namespace ast
{
    class method_definition : public node
    {
    public:
        method_definition(chime::parser* parser);
        
        virtual std::string node_name(void) const;
        virtual std::string to_string(void) const;
        
        std::string         identifier() const;
        void                identifier(std::string s);
        std::string         getName() const;
        NodeRef             getBody() const;
        ParameterSetRef     getParameters() const;
        
        llvm::Value*        codegen(chime::code_generator& generator);
        
    protected:
        std::string     _identifier;
        NodeRef         _bodyBlock;
        ParameterSetRef _parameters;
    };
}

#endif // METHOD_DEFINITON_H
