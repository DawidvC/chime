// implementation.cpp

#include "implementation.h"
#ifndef COMPILER_PCH_H
#    include "compiler/parser/parser.h"
#endif

namespace ast
{
    Implementation::Implementation(chime::parser& parser)
    {
        // parse the import statement
        parser.next_token_value("implementation");
        
        this->setTypeRef(TypeRef(new ast::type_reference(&parser)));
        
        // check for ": SuperClass"
        if (parser.advance_token_if_equals(":"))
        {
            this->setSuperclass(TypeRef(new ast::type_reference(&parser)));
        }
        else
        {
            this->setSuperclass(TypeRef());
        }
        
        // { body }
        parser.advance_past_ending_tokens();
        parser.next_token_value("{");
        
        _bodyBlock = new CodeBlock(parser, true);
        
        parser.advance_past_ending_tokens();
        parser.next_token_value("}");
    }
    
    Implementation::~Implementation()
    {
    }
    
    std::string Implementation::nodeName(void) const
    {
        return std::string("implementation");
    }
    
    std::string Implementation::stringRepresentation(int depth) const
    {
        std::string s;
        
        s.append(depth*2, ' ');
        s.append("implementation: ");
        s.append(this->getTypeRef()->identifier());
        
        if (this->getSuperclass().get())
            s += " : " + this->getSuperclass()->identifier();
        
        return s;
    }
    
    llvm::Function* Implementation::createInitFunction(chime::code_generator& generator)
    {
        llvm::Function*     initFunction;
        llvm::FunctionType* functionType;
        std::string         functionName;
        
        functionName = "init_" + this->getTypeRef()->identifier();
        
        functionType = generator.getRuntime()->getChimeModuleInitFunctionType();
        
        initFunction = generator.createFunction(functionType, functionName);
        
        return initFunction;
    }
    
    llvm::Value* Implementation::codegen(chime::code_generator& generator)
    {
        llvm::Value*                  classNamePtr;
        llvm::Value*                  superclassNamePtr;
        llvm::Value*                  objectClassPtr;
        llvm::Value*                  superclassPtr;
        llvm::Function*   initFunction;
        llvm::BasicBlock* basicBlock;
        llvm::BasicBlock* currentBlock;
        chime::ImplementationScopeRef scope;
        
        // capture the current block
        currentBlock = generator.builder()->GetInsertBlock();
        
        // create the class's initialization function, and add it to the
        // generator's list
        initFunction = this->createInitFunction(generator);
        
        generator.getInitFunctions()->push_back(initFunction);
        
        // setup our insertion point in the init function
        basicBlock = llvm::BasicBlock::Create(generator.getContext(), "entry", initFunction, 0);
        generator.builder()->SetInsertPoint(basicBlock);
        
        // first create the c-string name of the new class
        classNamePtr = generator.make_constant_string(this->getTypeRef()->identifier());
        
        // if we have a superclass, create a c-string name for the superclass,
        // and then get that class object
        if (this->getSuperclass().get())
        {
            superclassNamePtr = generator.make_constant_string(this->getSuperclass()->identifier());
        }
        else
        {
            superclassNamePtr = generator.make_constant_string("Object");
        }
        
        superclassPtr = generator.getRuntime()->callChimeRuntimeGetClass(superclassNamePtr);
        
        // now, create the new class
        objectClassPtr = generator.getRuntime()->callChimeRuntimeCreateClass(classNamePtr, superclassPtr);
        
        // with it created, we can now create a new implementation scope and assign it
        scope = chime::ImplementationScopeRef(new chime::ImplementationScope());
        
        scope->setTarget(objectClassPtr);
        scope->setName(this->getTypeRef()->identifier());
        
        generator.setImplementationScope(scope);
        
        // and now, finally, we can actually codegen the internals of the implementation
        _bodyBlock->codegen(generator);
        
        llvm::ReturnInst::Create(generator.getContext(), generator.builder()->GetInsertBlock());
        //generator.builder()->CreateRet();
        
        // verify the function
        llvm::verifyFunction(*initFunction);
        
        // restore the builder's position
        generator.builder()->SetInsertPoint(currentBlock);
        
        return NULL;
    }
    
    ast::TypeRef Implementation::getTypeRef() const
    {
        return _typeRef;
    }
    void Implementation::setTypeRef(ast::TypeRef node)
    {
        _typeRef = node;
    }
    
    ast::TypeRef Implementation::getSuperclass() const
    {
        return _superclass;
    }
    void Implementation::setSuperclass(ast::TypeRef node)
    {
        _superclass = node;
    }
    
    CodeBlock* Implementation::getBody(void) const
    {
        return _bodyBlock;
    }
}
