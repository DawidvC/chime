#include "Closure.h"
#include "compiler/ast/common/CodeBlock.h"
#include "compiler/ast/literals/ClosedSelfLiteral.h"
#include "compiler/ast/variable/LocalVariable.h"
#include "compiler/ast/variable/ClosedLocalVariable.h"
#include "compiler/codegen/code_generator.h"

namespace ast
{
    ast::Closure* Closure::parse(chime::parser& parser)
    {
        Closure* closure;
        
        parser.next_token_value("do");
        
        closure = new ast::Closure();
        
        parser.pushScope(closure);
        
        closure->setParameters(chime::ParameterSet::parse(parser));
        
        // This cast is currently necessary, even though now CodeBlock::parse will
        // always return an actual CodeBlockRef.  The signature of CodeBlock::parse should
        // really fixed, but that affects a lot of classes.
        closure->_bodyBlock = std::tr1::static_pointer_cast<CodeBlock>(CodeBlock::parse(parser));
        
        // before we pop the scope, we need to inform the enclosing scope of any
        // local variables that now need to be shared
        std::map<std::string, Variable*>::iterator it;
        
        for (it = closure->_closedVariables.begin(); it != closure->_closedVariables.end(); ++it)
        {
            closure->getParent()->capturedVariable(it->second);
        }
        
        parser.popScope();
        
        return closure;
    }
    
    Closure::Closure()
    {
        this->setIdentifier("Unset Closure Identifier");
    }
    
    std::string Closure::nodeName(void) const
    {
        return std::string("Closure");
    }
    
    std::string Closure::stringRepresentation(int depth) const
    {
        std::string str;
        
        str.append(depth*2, ' ');
        str.append("Closure");
        
        str.append("\n");
        
        str.append(this->getBody()->stringRepresentation(depth+1));
        
        return str;
    }
    
    CodeBlockRef Closure::getBody() const
    {
        return _bodyBlock;
    }
    
    std::vector<Variable*> Closure::getClosedVariables() const
    {
        // iterator must be const, since the method is marked const
        std::map<std::string, Variable*>::const_iterator it;
        std::vector<Variable*>                           variables;
        
        for (it = _closedVariables.begin(); it != _closedVariables.end(); ++it )
        {
            variables.push_back(it->second);
        }
        
        return variables;
    }
    
    chime::SelfLiteral* Closure::createSelf()
    {
        return new chime::ClosedSelfLiteral();
    }
    
    Variable* Closure::createVariable(const std::string& identifier)
    {
        //if (_closedVariables.find(identifier) != _closedVariables.end())
        //    return new ClosedLocalVariable(identifier);
        
        return new LocalVariable(identifier);
    }
    
    Variable* Closure::transformVariable(Variable* variable)
    {
        _closedVariables[variable->getIdentifier()] = variable;
        
        if (variable->nodeName() == "Local Variable")
        {
            variable = new ClosedLocalVariable(variable->getIdentifier());
        }
        
        return variable;
    }
    
    llvm::Value* Closure::codegen(chime::code_generator& generator)
    {
        llvm::Function* function;
        llvm::Value*    closureValue;
        std::string     name;
        llvm::Value*    referenceValue;
        
        name = generator.getCurrentScope()->getAnonymousFunctionName();
        this->setIdentifier(name);
        
        function = this->codegenFunction(generator, name, this->getBody(), this->getParameters()->length());
        
        closureValue = generator.getRuntime()->callChimeClosureCreate(function);
        
        // put in the closed values
        std::map<std::string, Variable*>::const_iterator it;
        
        for (it = _closedVariables.begin(); it != _closedVariables.end(); ++it)
        {
            llvm::Value* variableNameCStringPtr;
            
            variableNameCStringPtr = generator.getConstantString(it->first);
            referenceValue         = it->second->codegenReference(generator);
            
            generator.getCurrentScope()->setValueForIdentifier(it->first, referenceValue);
            
            generator.getRuntime()->callChimeObjectSetAttribute(closureValue, variableNameCStringPtr, referenceValue);
        }
        
        // finally, do self
        generator.getRuntime()->callChimeObjectSetAttribute(closureValue, generator.getConstantString("self"), generator.getCurrentScope()->getSelfObjectPtr());
        
        return closureValue;
    }
}