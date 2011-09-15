// chime: Closure.h

#ifndef CLOSURE_H
#define CLOSURE_H

#include "FunctionDefinition.h"
#include "compiler/ast/common/code_block.h"

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
        
        CodeBlockRef           getBody() const;
        std::vector<Variable*> getClosedVariables() const;
        
        Variable*    createVariable(const std::string& identifier);
        Variable*    transformVariable(Variable* variable);
        
        llvm::Function* createFunction(chime::code_generator& generator);
        void            codegenArguments(chime::code_generator& generator, llvm::Function* function);
        llvm::Function* codegenFunction(chime::code_generator& generator);
        
        llvm::Value*    codegen(chime::code_generator& generator);
    
    private:
        // parsing members
        CodeBlockRef _bodyBlock;
        
        // codegen members
        std::map<std::string, Variable*> _closedVariables;
    };
    
    typedef std::tr1::shared_ptr<Closure> ClosureRef;
}

#endif
