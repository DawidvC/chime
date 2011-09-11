#include "Closure.h"
#include "compiler/ast/common/code_block.h"
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
        
        if (parser.look_ahead()->equal_to("("))
        {
            closure->_parameters = ParameterSetRef(new ParameterSet(parser));
        }
        
        // this cast is probably not always safe because the current
        // implemenation of CodeBlock::parseNextBlock can return non-CodeBlockRef
        // types
        closure->_bodyBlock = std::tr1::static_pointer_cast<CodeBlock>(CodeBlock::parseNextBlock(parser));
        
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
    
    ParameterSetRef Closure::getParameters() const
    {
        return _parameters;
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
    
    Variable* Closure::createVariable(const std::string& identifier)
    {
        return new LocalVariable(identifier);
    }
    
    Variable* Closure::transformVariable(Variable* variable)
    {
        Variable* newVariable;
        
        newVariable = variable;
        
        if (variable->nodeName() == "Local Variable")
        {
            newVariable = new ClosedLocalVariable(variable->getIdentifier());
            
            delete variable;
        }
        
        _closedVariables[newVariable->getIdentifier()] = newVariable;
        
        return newVariable;
    }
    
    llvm::Value* Closure::codegenEnvironment(chime::code_generator& generator)
    {
        llvm::Value* environment;
        llvm::Value* hashClass;
        llvm::Value* string;
        
        string = generator.getConstantString("Hash");
        
        hashClass = generator.getRuntime()->callChimeRuntimeGetClass(string);
        
        string = generator.getConstantString("new");
        
        environment = generator.getRuntime()->callChimeObjectInvoke(hashClass, string, std::vector<llvm::Value*>());
        
        // put in the closed values
        std::map<std::string, Variable*>::iterator it;
        
        for (it = _closedVariables.begin(); it != _closedVariables.end(); ++it)
        {
            llvm::Value* variableValue;
            llvm::Value* variableNameCStringPtr;
            
            variableValue          = generator.getCurrentScope()->getValueForIdentifier(it->second->getIdentifier());
            variableNameCStringPtr = generator.getConstantString(it->first);
            
            generator.getRuntime()->callChimeObjectSetAttribute(environment, variableNameCStringPtr, variableValue);
        }
        
        return environment;
    }
    
    llvm::Value* Closure::codegen(chime::code_generator& generator)
    {
        // This code is very very similar to the code in FunctionDefinition.cpp.  I'd like to
        // figure out a way to share it better
        
        llvm::Function*                function;
        llvm::FunctionType*            functionType;
        std::vector<const llvm::Type*> functionArgs;
        std::string                    functionName;
        llvm::Value*                   closureValue;
        llvm::BasicBlock*              basicBlock;
        llvm::BasicBlock*              currentBlock;
        
        // setup the function type
        functionArgs.push_back(generator.getRuntime()->getChimeObjectPtrType()); // this is self (the closure object)
        
        // capture the current insertion point
        currentBlock = generator.builder()->GetInsertBlock();
        
        functionName = generator.getMethodScope()->createNewAnonymousFunctionName();
        
        functionType = llvm::FunctionType::get(generator.getRuntime()->getChimeObjectPtrType(), functionArgs, false);
        
        function = generator.createFunction(functionType, functionName);
        
        generator.setMethodScope(chime::MethodScopeRef(new chime::MethodScope()));
        generator.getMethodScope()->setName(functionName);
        generator.pushScope(this);
        
        basicBlock = llvm::BasicBlock::Create(generator.getContext(), "entry", function, 0);
        generator.builder()->SetInsertPoint(basicBlock);
        
        // parameters
        llvm::Function::arg_iterator args;
        llvm::AllocaInst*            alloca;
        llvm::Value*                 environment;
        
        // first, deal with self
        args = function->arg_begin();
        
        alloca = generator.insertChimeObjectAlloca();
        generator.builder()->CreateStore(args, alloca, false);
        
        generator.getCurrentScope()->setValueForIdentifier("_self", alloca);
        
        // now body
        this->getBody()->codegen(generator);
        
        // only do this if we haven't yet terminated the block
        if (!generator.builder()->GetInsertBlock()->getTerminator())
        {
            generator.builder()->CreateRet(generator.getRuntime()->getChimeLiteralNull());
        }
        
        llvm::verifyFunction(*function);
        
        // restore the builder's position
        generator.popScope();
        generator.builder()->SetInsertPoint(currentBlock);
        
        closureValue = generator.getRuntime()->callChimeClosureCreate(function);
        environment  = this->codegenEnvironment(generator);
        
        generator.getRuntime()->callChimeClosureSetEnvironment(closureValue, environment);
        generator.getCurrentScope()->setValueForIdentifier("_environment", environment);
        
        return closureValue;
    }
}