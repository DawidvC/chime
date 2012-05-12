// chime: PropertyDefinition.h

#ifndef PROPERTY_DEFINITON_H
#define PROPERTY_DEFINITON_H

#include "FunctionDefinition.h"
#include "compiler/ast/CodeBlock.h"

namespace chime
{
    class PropertyDefinition : public FunctionDefinition
    {
    public:
        static PropertyDefinition* parse(chime::parser& parser);
    
    public:
        virtual std::string nodeName() const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        chime::CodeBlockRef getGetBody() const;
        chime::CodeBlockRef getSetBody() const;
        
        chime::Variable* createVariable(const std::string& identifier);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
        
    protected:
        chime::CodeBlockRef _getBodyBlock;
        chime::CodeBlockRef _setBodyBlock;
    };
}

#endif // PROPERTY_DEFINITON_H
