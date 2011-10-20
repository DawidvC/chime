// chime: method_definition.h

#ifndef METHOD_DEFINITON_H
#define METHOD_DEFINITON_H

#include "FunctionDefinition.h"

namespace ast
{
    class method_definition : public FunctionDefinition
    {
    public:
        static method_definition* parse(chime::parser& parser);
    
    public:
        method_definition();
        
        virtual std::string nodeName(void) const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        NodeRef             getBody() const;
        
        Variable*           createVariable(const std::string& identifier);
        
        llvm::Value* codegen(chime::code_generator& generator);
        
    protected:
        NodeRef _bodyBlock;
    };
}

namespace chime
{
    typedef ast::method_definition MethodDefinition;
}

#endif // METHOD_DEFINITON_H
