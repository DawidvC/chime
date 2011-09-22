// chime: MethodCall.h

#ifndef METHOD_CALL_H
#define METHOD_CALL_H

#include "basic_operator.h"

namespace ast
{
    class MethodCall : public basic_operator
    {
    public:
        static ast::Node* parse(chime::parser& parser);
        static void parseArguments(chime::parser& parser, ast::MethodCall* call);
    
    public:
        std::string nodeName(void) const;
        std::string stringRepresentation(int depth=0) const;
        
        llvm::Value* codegenWithTarget(chime::code_generator& generator, llvm::Value* target);
    };
}

#endif // METHOD_CALL_H
