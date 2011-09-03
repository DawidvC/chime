#include "Closure.h"
#include "compiler/ast/common/code_block.h"
#include "compiler/ast/variable/LocalVariable.h"
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
    
    void Closure::definedInParent(Variable* variable)
    {
        variable->setClosed(true);
        
        _closedVariables[variable->getIdentifier()] = variable;
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
        
        // test for determining closed variables
        std::vector<std::string>           names(this->getContainedVariableNames());
        std::vector<std::string>::iterator i;
        
        fprintf(stderr, "Listing closed variables in closure\n");
        for (i = names.begin(); i < names.end(); ++i)
        {
            fprintf(stderr, "Contained: %s\n", i->c_str());
        }
        fprintf(stderr, "Listed closed variables in closure\n");
        
        // capture the current insertion point
        currentBlock = generator.builder()->GetInsertBlock();
        
        functionName = generator.getMethodScope()->createNewAnonymousFunctionName();
        
        // returns nothing, takes no arguments
        functionType = llvm::FunctionType::get(generator.getRuntime()->getChimeObjectPtrType(), functionArgs, false);
        
        function = generator.createFunction(functionType, functionName);
        
        basicBlock = llvm::BasicBlock::Create(generator.getContext(), "entry", function, 0);
        generator.builder()->SetInsertPoint(basicBlock);
        
        this->getBody()->codegen(generator);
        
        // only do this if we haven't yet terminated the block
        if (!generator.builder()->GetInsertBlock()->getTerminator())
        {
            generator.builder()->CreateRet(generator.getRuntime()->getChimeLiteralNull());
        }
        
        llvm::verifyFunction(*function);
        
        // restore the builder's position
        generator.builder()->SetInsertPoint(currentBlock);
        
        closureValue = generator.getRuntime()->callChimeClosureCreate(function);
        
        return closureValue;
    }
}