// implementation.cpp

#include "implementation.h"
#include "compiler/ast/variable/InstanceVariable.h"
#include "compiler/parser/parser.h"

namespace ast
{
    Implementation* Implementation::parse(chime::parser& parser)
    {
        Implementation* implementation;
        
        implementation = new Implementation();
        
        parser.pushScope(implementation);
        
        // parse the import statement
        parser.next_token_value("implementation");
        
        implementation->setTypeRef(chime::TypeRef(new chime::Type(&parser)));
        
        // check for ": SuperClass"
        if (parser.advance_token_if_equals(":"))
        {
            implementation->setSuperclass(chime::TypeRef(new chime::Type(&parser)));
        }
        else
        {
            implementation->setSuperclass(chime::TypeRef());
        }
        
        // parse the body, allowing structural elements inside
        implementation->_bodyBlock = chime::CodeBlock::parse(parser, true);
        
        parser.popScope();
        
        return implementation;
    }
    
    std::string Implementation::nodeName(void) const
    {
        return std::string("implementation");
    }
    
    std::string Implementation::stringRepresentation(int depth) const
    {
        std::string str;
        
        str.append(depth*2, ' ');
        str.append("implementation: ");
        str.append(this->getTypeRef()->identifier());
        
        if (this->getSuperclass().get())
            str += " : " + this->getSuperclass()->identifier();
        
        str.append("\n");
        str.append(this->getBody()->stringRepresentation(depth+1));
        
        return str;
    }
    
    chime::TypeRef Implementation::getTypeRef() const
    {
        return _typeRef;
    }
    void Implementation::setTypeRef(chime::TypeRef node)
    {
        _typeRef = node;
    }
    
    chime::TypeRef Implementation::getSuperclass() const
    {
        return _superclass;
    }
    void Implementation::setSuperclass(chime::TypeRef node)
    {
        _superclass = node;
    }
    
    NodeRef Implementation::getBody(void) const
    {
        return _bodyBlock;
    }
    
    std::string Implementation::getIdentifier() const
    {
        return this->getTypeRef()->identifier();
    }
    
    chime::Variable* Implementation::createVariable(const std::string& identifier)
    {
        return new InstanceVariable(identifier);
    }
    
    bool Implementation::allowsStructuralElements() const
    {
        return true;
    }
    
    llvm::Function* Implementation::createInitFunction(chime::code_generator& generator)
    {
        llvm::Function*     initFunction;
        llvm::FunctionType* functionType;
        std::string         functionName;
        
        functionName = "init_implementation_" + this->getTypeRef()->identifier();
        
        functionType = generator.getRuntime()->getChimeModuleInitFunctionType();
        
        initFunction = generator.createFunction(functionType, functionName);
        
        return initFunction;
    }
    
    llvm::Value* Implementation::codegen(chime::code_generator& generator)
    {
        llvm::Value*      classNamePtr;
        llvm::Value*      superclassNamePtr;
        llvm::Value*      objectClassPtr;
        llvm::Value*      superclassPtr;
        llvm::Function*   initFunction;
        llvm::BasicBlock* basicBlock;
        llvm::BasicBlock* currentBlock;
        
        initFunction = this->createInitFunction(generator);
        generator.builder()->CreateCall(initFunction, "");
        
        // capture the current block
        currentBlock = generator.builder()->GetInsertBlock();
        
        // setup our insertion point in the init function
        basicBlock = llvm::BasicBlock::Create(generator.getContext(), "entry", initFunction, 0);
        generator.builder()->SetInsertPoint(basicBlock);
        
        // first create the c-string name of the new class
        classNamePtr = generator.getConstantString(this->getTypeRef()->identifier());
        
        // if we have a superclass, create a c-string name for the superclass,
        // and then get that class object
        if (this->getSuperclass().get())
        {
            superclassNamePtr = generator.getConstantString(this->getSuperclass()->identifier());
        }
        else
        {
            superclassNamePtr = generator.getConstantString("Object");
        }
        
        superclassPtr = generator.getRuntime()->callChimeRuntimeGetClass(superclassNamePtr);
        
        // now, create the new class
        objectClassPtr = generator.getRuntime()->callChimeRuntimeCreateClass(classNamePtr, superclassPtr);
        this->setClassObjectPtr(objectClassPtr);
        this->setSelfObjectPtr(objectClassPtr); // it's not 100% clear if this is the correct thing to do
        
        generator.pushScope(this);
        
        // and now, finally, we can actually codegen the internals of the implementation
        _bodyBlock->codegen(generator);
        
        generator.popScope();
        
        generator.builder()->CreateRetVoid();
        
        // verify the function
        llvm::verifyFunction(*initFunction);
        
        // restore scope and the builder's position
        generator.builder()->SetInsertPoint(currentBlock);
        
        return NULL;
    }
}
