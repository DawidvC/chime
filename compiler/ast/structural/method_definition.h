// chime: method_definition.h

#ifndef METHOD_DEFINITON_H
#define METHOD_DEFINITON_H

#include "FunctionDefinition.h"

namespace chime
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
        
        void         codegenNewMethod(chime::code_generator& generator, llvm::Value* initializeFunction);
        llvm::Value* codegen(chime::code_generator& generator);
        
    protected:
        NodeRef _bodyBlock;
    };
    
    typedef chime::method_definition MethodDefinition;
}

#endif // METHOD_DEFINITON_H
