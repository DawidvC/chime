// chime: FunctionDefinition.h

#ifndef FUNCTION_DEFINITON_H
#define FUNCTION_DEFINITON_H

#include "compiler/ast/LocalScopedNode.h"
#include "compiler/ast/literals/SelfLiteral.h"
#include "compiler/ast/variable/Parameter.h"

namespace chime
{
    class FunctionDefinition : public chime::LocalScopedNode
    {
    public:
        FunctionDefinition();
        
        std::string     baseIdentifier() const;
        std::string     getIdentifier() const;
        void            setIdentifier(const std::string& s);
        std::vector<chime::ParameterRef> getParameters() const;
        void            setParameters(const std::vector<chime::ParameterRef>& params);
        bool            isInstance() const;
        void            setInstance(bool value);
        
        chime::SelfLiteral* createSelf();
        
        bool                isFunction() const;
        
    protected:
        llvm::Function* createFunction(chime::code_generator& generator, const std::string& name, unsigned int arity);
        void            codegenArguments(chime::code_generator& generator, llvm::Function* function);
        llvm::Function* codegenFunction(chime::code_generator& generator, const std::string& name, NodeRef body, unsigned int arity);
        
        llvm::Value*    createMethod(chime::code_generator& generator, const std::string& name, NodeRef body, unsigned int arity);
        
    private:
        std::string     _identifier;
        std::vector<chime::ParameterRef> _parameters;
        bool            _isInstance;
    };
}

#endif // FUNCTION_DEFINITON_H
