// chime: MethodCall.h

#ifndef METHOD_CALL_H
#define METHOD_CALL_H

#include "basic_operator.h"

namespace chime
{
    class MethodCall : public basic_operator
    {
    public:
        static Node* parse(chime::parser& parser);
        static void  parseArguments(chime::parser& parser, MethodCall* call);
    
    public:
        std::string nodeName(void) const;
        std::string stringRepresentation(int depth=0) const;
        
        llvm::Value* codegenWithTarget(chime::code_generator& generator, llvm::Value* target);
    };
}

#endif // METHOD_CALL_H
