#include "Trait.h"
#include "compiler/ast/variable/InstanceVariable.h"
#include "compiler/ast/common/CodeBlock.h"

namespace chime
{
    Trait* Trait::parse(parser& parser)
    {
        Trait* trait;
        
        trait = new Trait();
        
        parser.pushScope(trait);
        
        // parse the import statement
        parser.nextTokenValue("trait");
        
        trait->_typeRef = TypeRef(new Type(&parser));
        
        // parse the body, allowing structural elements inside
        trait->_bodyBlock = ast::CodeBlock::parse(parser, true);
        
        parser.popScope();
        
        return trait;
    }
    
    std::string Trait::nodeName() const
    {
        return "Trait";
    }
    
    std::string Trait::stringRepresentation(int depth) const
    {
        std::string str;
        
        str.append(depth*2, ' ');
        str.append("trait: ");
        str.append(this->getTypeRef()->identifier());
        
        str.append("\n");
        str.append(this->getBody()->stringRepresentation(depth+1));
        
        return str;
    }
    
    TypeRef Trait::getTypeRef() const
    {
        return _typeRef;
    }
    
    NodeRef Trait::getBody() const
    {
        return _bodyBlock;
    }
    
    std::string Trait::getIdentifier() const
    {
        return this->getTypeRef()->identifier();
    }
    
    Variable* Trait::createVariable(const std::string& identifier)
    {
        return new ast::InstanceVariable(identifier);
    }
    
    bool Trait::allowsStructuralElements() const
    {
        return true;
    }
    
    llvm::Function* Trait::createInitFunction(CodeGenContext& context)
    {
        llvm::Function*     initFunction;
        llvm::FunctionType* functionType;
        std::string         functionName;
        
        functionName = "init_trait_" + this->getIdentifier();
        
        functionType = context.getRuntime()->getChimeModuleInitFunctionType();
        
        initFunction = context.createFunction(functionType, functionName);
        
        return initFunction;
    }
    
    llvm::Value* Trait::codegen(CodeGenContext& context)
    {
        llvm::Value*      traitNamePtr;
        llvm::Value*      traitPtr;
        llvm::Function*   initFunction;
        llvm::BasicBlock* basicBlock;
        llvm::BasicBlock* currentBlock;
        
        initFunction = this->createInitFunction(context);
        context.builder()->CreateCall(initFunction, "");
        
        // create the class's initialization function, and add it to the
        // context's list
        currentBlock = context.builder()->GetInsertBlock();
        
        // setup our insertion point in the init function
        basicBlock = llvm::BasicBlock::Create(context.getContext(), "entry", initFunction, 0);
        context.builder()->SetInsertPoint(basicBlock);
        
        // first create the c-string name of the new trait
        traitNamePtr = context.getConstantString(this->getIdentifier());
        
        // now, create the new class
        traitPtr = context.getRuntime()->callChimeTraitCreate(traitNamePtr);
        this->setClassObjectPtr(traitPtr);
        
        context.pushScope(this);
        
        // and now, finally, we can actually codegen the internals of the implementation
        _bodyBlock->codegen(context);
        
        context.popScope();
        
        context.builder()->CreateRetVoid();
        
        // verify the function
        llvm::verifyFunction(*initFunction);
        
        // restore scope and the builder's position
        context.builder()->SetInsertPoint(currentBlock);
        
        return NULL;
    }
}
