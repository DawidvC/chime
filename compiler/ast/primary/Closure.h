#ifndef CLOSURE_H
#define CLOSURE_H

#include "compiler/ast/ScopedNode.h"
#include "compiler/ast/common/code_block.h"
#include "compiler/ast/common/parameter_set.h"

#include <map>

namespace ast
{
    class Closure : public ScopedNode
    {
    public:
        static ast::Closure* parse(chime::parser& parser);
    
    public:
        Closure();
        
        virtual std::string    nodeName(void) const;
        std::string            stringRepresentation(int depth=0) const;
        
        CodeBlockRef           getBody() const;
        ParameterSetRef        getParameters() const;
        std::vector<Variable*> getClosedVariables() const;
        
        Variable*    createVariable(const std::string& identifier);
        Variable*    transformVariable(Variable* variable);
        
        llvm::Value* codegen(chime::code_generator& generator);
    
    private:
        CodeBlockRef    _bodyBlock;
        ParameterSetRef _parameters;
        
        std::map<std::string, Variable*> _closedVariables;
    };
    
    typedef std::tr1::shared_ptr<Closure> ClosureRef;
}

#endif
