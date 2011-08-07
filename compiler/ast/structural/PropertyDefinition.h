#ifndef PROPERTY_DEFINITON_H
#define PROPERTY_DEFINITON_H

#include "compiler/ast/node.h"
#include "compiler/ast/common/parameter_set.h"
#include "compiler/ast/common/code_block.h"

namespace ast
{
    class PropertyDefinition : public node
    {
    public:
        PropertyDefinition(chime::parser& parser);
        
        virtual std::string nodeName() const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        std::string         getIdentifier() const;
        CodeBlockRef        getGetBody() const;
        CodeBlockRef        getSetBody() const;
        ParameterSetRef     getParameters() const;
        
        llvm::Value*        codegen(chime::code_generator& generator);
        
    protected:
        std::string     _identifier;
        CodeBlockRef    _getBodyBlock;
        CodeBlockRef    _setBodyBlock;
        ParameterSetRef _parameters;
    };
}

#endif // PROPERTY_DEFINITON_H
