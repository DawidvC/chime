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
    
    ast::Variable* Trait::createVariable(const std::string& identifier)
    {
        return new ast::InstanceVariable(identifier);
    }
    
    llvm::Function* Trait::createInitFunction(CodeGenContext& context)
    {
        llvm::Function*     initFunction;
        llvm::FunctionType* functionType;
        std::string         functionName;
        
        functionName = "init_" + this->getIdentifier();
        
        functionType = context.getRuntime()->getChimeModuleInitFunctionType();
        
        initFunction = context.createFunction(functionType, functionName);
        
        return initFunction;
    }
    
    llvm::Value* Trait::codegen(CodeGenContext& context)
    {
        llvm::Value*                  traitNamePtr;
        llvm::Value*                  traitPtr;
        llvm::Function*               initFunction;
        llvm::BasicBlock*             basicBlock;
        llvm::BasicBlock*             currentBlock;
        chime::ImplementationScopeRef scope;
        
        // create the class's initialization function, and add it to the
        // context's list
        currentBlock = context.builder()->GetInsertBlock();
        
        initFunction = this->createInitFunction(context);
        
        context.getInitFunctions()->push_back(initFunction);
        
        // setup our insertion point in the init function
        basicBlock = llvm::BasicBlock::Create(context.getContext(), "entry", initFunction, 0);
        context.builder()->SetInsertPoint(basicBlock);
        
        // first create the c-string name of the new trait
        traitNamePtr = context.getConstantString(this->getIdentifier());
        
        // now, create the new class
        traitPtr = context.getRuntime()->callChimeTraitCreate(traitNamePtr);
        
        // with it created, we can now create a new implementation scope and assign it
        scope = chime::ImplementationScopeRef(new chime::ImplementationScope());
        
        scope->setTarget(traitPtr);
        scope->setName(this->getIdentifier());
        
        context.setImplementationScope(scope);
        context.pushScope(this);
        
        // and now, finally, we can actually codegen the internals of the implementation
        _bodyBlock->codegen(context);
        
        llvm::ReturnInst::Create(context.getContext(), context.builder()->GetInsertBlock());
        
        // verify the function
        llvm::verifyFunction(*initFunction);
        
        // restore scope and the builder's position
        context.popScope();
        context.builder()->SetInsertPoint(currentBlock);
        
        return NULL;
    }
}
