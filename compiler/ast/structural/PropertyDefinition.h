// chime: PropertyDefinition.h

#ifndef PROPERTY_DEFINITON_H
#define PROPERTY_DEFINITON_H

#include "FunctionDefinition.h"
#include "compiler/ast/common/CodeBlock.h"

namespace ast
{
    class PropertyDefinition : public FunctionDefinition
    {
    public:
        static PropertyDefinition* parse(chime::parser& parser);
    
    public:
        virtual std::string nodeName() const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        CodeBlockRef getGetBody() const;
        CodeBlockRef getSetBody() const;
        
        chime::Variable* createVariable(const std::string& identifier);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
        
    protected:
        CodeBlockRef _getBodyBlock;
        CodeBlockRef _setBodyBlock;
    };
}

#endif // PROPERTY_DEFINITON_H
