// chime: Closure.h

#ifndef CLOSURE_H
#define CLOSURE_H

#include "FunctionDefinition.h"
#include "compiler/ast/CodeBlock.h"
#include "compiler/ast/literals/SelfLiteral.h"

#include <map>

namespace ast
{
    class Closure : public FunctionDefinition
    {
    public:
        static ast::Closure* parse(chime::parser& parser);
    
    public:
        Closure();
        
        virtual std::string    nodeName(void) const;
        std::string            stringRepresentation(int depth=0) const;
        
        chime::CodeBlockRef           getBody() const;
        std::vector<chime::Variable*> getClosedVariables() const;
        
        chime::SelfLiteral*    createSelf();
        llvm::Value*           selfValue(chime::CodeGenContext& context);
        
        chime::Variable*    createVariable(const std::string& identifier);
        chime::Variable*    transformVariable(chime::Variable* variable);
        
        llvm::Value* codegen(chime::code_generator& generator);
    
    private:
        // parsing members
        chime::CodeBlockRef _bodyBlock;
        
        // codegen members
        std::map<std::string, chime::Variable*> _closedVariables;
    };
    
    typedef shared_ptr<Closure> ClosureRef;
}

#endif
