#include "Closure.h"
#include "compiler/ast/CodeBlock.h"
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
        
        closure->setParameters(chime::Parameter::parseList(parser));
        
        // This cast is currently necessary, even though now CodeBlock::parse will
        // always return an actual CodeBlockRef.  The signature of CodeBlock::parse should
        // really fixed, but that affects a lot of classes.
        closure->_bodyBlock = dynamic_pointer_cast<chime::CodeBlock>(chime::CodeBlock::parse(parser));
        
        // I originally missed this setDeferToParent case, and had a really hard time finding it.  But, after
        // a bunch of attempts, I was unable to produce a good, reduced test case.  I'm going to just leave it
        // as fixed, though I still feel like it really should be properly captured in a test.
        
        // just like regular functions, these blocks need to defer to their parents
        dynamic_pointer_cast<chime::CodeBlock>(closure->_bodyBlock)->setDeferToParent(true);
        
        // before we pop the scope, we need to inform the enclosing scope of any
        // local variables that now need to be shared
        std::map<std::string, chime::Variable*>::const_iterator it;
        
        for (it = closure->_closedVariables.begin(); it != closure->_closedVariables.end(); ++it)
        {
            closure->parent()->capturedVariable(it->second);
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
    
    chime::CodeBlockRef Closure::getBody() const
    {
        return _bodyBlock;
    }
    
    std::vector<chime::Variable*> Closure::getClosedVariables() const
    {
        // iterator must be const, since the method is marked const
        std::map<std::string, chime::Variable*>::const_iterator it;
        std::vector<chime::Variable*>                           variables;
        
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
    
    llvm::Value* Closure::selfValue(chime::CodeGenContext& context)
    {
        llvm::Value* closureValue;
        
        closureValue = context.getCurrentScope()->selfObjectPtr();
        
        assert(closureValue);
        
        return context.getRuntime()->callChimeObjectGetAttribute(closureValue, context.getConstantString("self"));
    }
    
    chime::Variable* Closure::createVariable(const std::string& identifier)
    {
        return new LocalVariable(identifier);
    }
    
    chime::Variable* Closure::transformVariable(chime::Variable* variable)
    {
        _closedVariables[variable->getIdentifier()] = variable;
        
        if (variable->requiresCapture())
        {
            bool defined;
            
            defined = variable->isDefined();
            
            variable = variable->createClosedVersion();
            variable->setDefined(defined);
        }
        
        return variable;
    }
    
    llvm::Value* Closure::codegen(chime::code_generator& generator)
    {
        llvm::Function* function;
        llvm::Value*    closureValue;
        std::string     name;
        llvm::Value*    referenceValue;
        
        // create the function name, based on the current function
        name = generator.getCurrentScope()->getAnonymousFunctionName();
        this->setIdentifier(name);
        
        // create the actual function
        function = this->codegenFunction(generator, name, this->getBody(), this->getParameters().size());
        
        closureValue = generator.getRuntime()->callChimeClosureCreate(function);
        
        // put in the closed values
        std::map<std::string, chime::Variable*>::const_iterator it;
        
        for (it = _closedVariables.begin(); it != _closedVariables.end(); ++it)
        {
            llvm::Value* variableNameCStringPtr;
            
            variableNameCStringPtr = generator.getConstantString(it->first);
            referenceValue         = it->second->codegenReference(generator);
            
            // fprintf(stderr, "<%s> closed variable: %s\n", this->getIdentifier().c_str(), it->first.c_str());
            
            generator.getCurrentScope()->setValueForIdentifier(it->first, referenceValue);
            
            generator.getRuntime()->callChimeObjectSetAttribute(closureValue, variableNameCStringPtr, referenceValue);
            
            // no release is needed here - taken care of in codegenReference
        }
        
        // finally, do self
        generator.getRuntime()->callChimeObjectSetAttribute(closureValue, generator.getConstantString("self"), generator.getCurrentScope()->selfValue(generator));
        
        // now, the closure needs to be tracked for release, just like any other value
        generator.getCurrentScope()->addLooseValue(closureValue);
        
        return closureValue;
    }
}