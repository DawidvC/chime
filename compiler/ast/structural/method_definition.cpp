// chime: method_definition.cpp

#include "method_definition.h"
#include "compiler/parser/parser.h"
#include "compiler/codegen/code_generator.h"
#include "compiler/ast/CodeBlock.h"
#include "compiler/ast/variable/LocalVariable.h"
#include "compiler/ast/variable/SharedLocalVariable.h"

namespace ast
{
    method_definition* method_definition::parse(chime::parser& parser)
    {
        method_definition* definition;
        std::string        identifier;
        
        definition = new ast::method_definition();
        
        parser.pushScope(definition);
        
        // "method identifier"
        parser.next_token_value("method");
        
        identifier = parser.nextTokenValue();
        if (parser.advanceTokenIfEqual("="))
        {
            // this handles the "method setter=(value)" case
            identifier += "=";
        }
        
        definition->setParameters(chime::Parameter::parseList(parser));
        
        // create the identifier
        if (definition->getParameters().size() > 0)
            identifier.append(":");
        
        for (unsigned int i = 1; i < definition->getParameters().size(); ++i)
        {
            chime::ParameterRef param;
            
            param = definition->getParameters()[i];
            
            identifier.append(param->label());
            identifier.append(":");
        }
        
        definition->setIdentifier(identifier);
        
        definition->_bodyBlock = chime::CodeBlock::parse(parser);
        
        // This is a little bit weird, but certain blocks need to not act like scopes.  Method scopes
        // are one of those cases.
        dynamic_pointer_cast<chime::CodeBlock>(definition->_bodyBlock)->setDeferToParent(true);
        
        parser.popScope();
        
        return definition;
    }
    
    method_definition::method_definition()
    {
    }
    
    std::string method_definition::nodeName(void) const
    {
        return std::string("method definition");
    }
    std::string method_definition::stringRepresentation(int depth) const
    {
        std::string str;
        
        str.append(depth*2, ' ');
        str.append("method: ");
        str.append(this->getIdentifier());
        
        str.append("\n");
        
        str.append(this->getBody()->stringRepresentation(depth+1));
        
        return str;
    }
    
    NodeRef method_definition::getBody(void) const
    {
        return _bodyBlock;
    }
    
    chime::Variable* method_definition::createVariable(const std::string& identifier)
    {
        // if this variable has been captured, at this point in the state of
        // parsing, then we need to do something special
        if (this->capturedIdentifier(identifier))
        {
            return new SharedLocalVariable(identifier);
        }
        
        return new LocalVariable(identifier);
    }
    
    void method_definition::codegenNewMethod(chime::code_generator& generator, llvm::Value* initializeFunction)
    {
        llvm::Function*   function;
        std::string       functionName;
        llvm::Value*      functionNameCStringPtr;
        llvm::Value*      classObjectPtr;
        llvm::BasicBlock* basicBlock;
        llvm::BasicBlock* currentBlock;
        llvm::Value*      objectValue;
        
        assert(this->baseIdentifier() == "initialize");
        
        // replace the first ten characters "initialize" with "new"
        functionName           = this->getIdentifier().replace(0, 10, "new");
        functionNameCStringPtr = generator.getConstantString(functionName);
        functionName           = generator.getCurrentScope()->getIdentifier() + "." + functionName;
        
        classObjectPtr         = generator.getCurrentScope()->classObjectPtr();
        
        // now create the actual function
        function = this->createFunction(generator, functionName, this->getParameters().size());
        
        // capture the current insertion point
        currentBlock = generator.builder()->GetInsertBlock();
        
        basicBlock = llvm::BasicBlock::Create(generator.getContext(), "entry", function, 0);
        generator.builder()->SetInsertPoint(basicBlock);
        
        // create the object
        // call initialize on it with the right arguments
        // return it
        
        llvm::Function::arg_iterator args;
        std::vector<llvm::Value*>    arguments;
        llvm::AllocaInst*            alloca;
        // llvm::LoadInst*              objectLoad;
        
        args = function->arg_begin();
        
        alloca = generator.insertChimeObjectAlloca();
        generator.builder()->CreateStore(args, alloca, false);
        
        objectValue = generator.getRuntime()->callChimeObjectCreate(alloca);
        
        ++args; // advance past self argument
        
        for (; args != function->arg_end(); ++args)
        {
            // llvm::AllocaInst* alloca;
            // 
            // alloca = generator.insertChimeObjectAlloca();
            // 
            // generator.builder()->CreateStore(args, alloca, false);
            // 
            // objectLoad = generator.builder()->CreateLoad(argumentValue, "loaded argument");
            
            arguments.push_back(args);
        }
        
        generator.getRuntime()->callChimeObjectInvoke(objectValue, generator.getConstantString(this->getIdentifier()), arguments);
        
        objectValue = generator.builder()->CreateLoad(objectValue, "return value");
        
        generator.builder()->CreateRet(objectValue); // return the newly created object
        
        llvm::verifyFunction(*function);
        
        // restore the builder's position
        generator.builder()->SetInsertPoint(currentBlock);
        
        // finally, install the method in the class
        generator.getRuntime()->callChimeRuntimeSetClassMethod(classObjectPtr, functionNameCStringPtr, function);
    }
    
    llvm::Value* method_definition::codegen(chime::code_generator& generator)
    {
        llvm::Value* function;
        
        function = this->createMethod(generator, this->getIdentifier(), this->getBody(), this->getParameters().size());
        
        if (this->isInstance() && (this->baseIdentifier() == "initialize"))
        {
            this->codegenNewMethod(generator, function);
        }
        
        return function;
    }
}
