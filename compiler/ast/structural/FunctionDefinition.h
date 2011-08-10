// chime: FunctionDefinition.h

#ifndef FUNCTION_DEFINITON_H
#define FUNCTION_DEFINITON_H

#include "compiler/ast/node.h"
#include "compiler/ast/common/parameter_set.h"
#include "compiler/ast/common/code_block.h"

namespace ast
{
    class FunctionDefinition : public node
    {
    public:
        FunctionDefinition();
        
        std::string         getIdentifier() const;
        void                setIdentifier(const std::string& s);
        ParameterSetRef     getParameters() const;
        void                setParameters(ParameterSetRef params);
        
    protected:
        llvm::Value*        createMethod(chime::code_generator& generator, const std::string& name, NodeRef body, unsigned int arity);
        
    private:
        std::string     _identifier;
        ParameterSetRef _parameters;
    };
}

#endif // FUNCTION_DEFINITON_H
