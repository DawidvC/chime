#include "Root.h"
#include "compiler/ast/variable/GlobalVariable.h"

namespace ast
{
    Root::Root()
    {
        _identifier = "Root";
    }
    
    std::vector<std::string> Root::getBinaryDependencies(void) const
    {
        return _binaryDependencies;
    }
    void Root::addBinaryDependency(const std::string& dependency)
    {
        _binaryDependencies.push_back(dependency);
    }
    
    std::vector<std::string> Root::getSourceDependencies(void) const
    {
        return _sourceDependencies;
    }
    void Root::addSourceDependency(const std::string& dependency)
    {
        _sourceDependencies.push_back(dependency);
    }
    
    std::string Root::getIdentifier() const
    {
        return _identifier;
    }
    
    void Root::setIdentifier(const std::string& identifier)
    {
        _identifier = identifier;
    }
    
    Variable* Root::createVariable(const std::string& identifier)
    {
        return new GlobalVariable(identifier);
    }
    
    chime::SelfLiteral* Root::createSelf()
    {
        return new chime::SelfLiteral();
    }
    
    bool Root::allowsStructuralElements() const
    {
        return true;
    }
    
    llvm::Value* Root::codegen(chime::CodeGenContext& context)
    {
        llvm::Value*                      cStringPtr;
        llvm::Value*                      moduleClassPtr;
        llvm::Value*                      classPtr;
        llvm::Value*                      moduleObjectPtr;
        std::vector<ast::node*>::iterator it;
        
        assert(context.getCurrentScope()->allowsStructuralElements());
        
        cStringPtr      = context.getConstantString("Module");
        moduleClassPtr  = context.getRuntime()->callChimeRuntimeGetClass(cStringPtr);
        
        cStringPtr      = context.getConstantString(this->getIdentifier());
        classPtr        = context.getRuntime()->callChimeRuntimeCreateClass(cStringPtr, moduleClassPtr);
        
        this->setClassObjectPtr(classPtr);
        
        //context.pushScope(this);
        
        cStringPtr      = context.getConstantString("new");
        moduleObjectPtr = context.getRuntime()->callChimeObjectInvoke(classPtr, cStringPtr, std::vector<llvm::Value*>());
        
        this->setSelfObjectPtr(moduleObjectPtr);
        
        for (it = this->children()->begin(); it < this->children()->end(); ++it)
        {
            (*it)->codegen(context);
        }
        
        // restore scope and the builder's position
        //context.popScope();
        
        return NULL;
    }
}
