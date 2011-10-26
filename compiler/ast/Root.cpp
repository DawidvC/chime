#include "Root.h"
#include "compiler/ast/variable/GlobalVariable.h"

namespace ast
{
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
        return std::string("Root");
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
        llvm::Value*                      moduleObjectPtr;
        std::vector<ast::node*>::iterator it;
        
        assert(context.getCurrentScope()->allowsStructuralElements());
        
        cStringPtr      = context.getConstantString("Module");
        moduleClassPtr  = context.getRuntime()->callChimeRuntimeGetClass(cStringPtr);
        
        cStringPtr      = context.getConstantString("new");
        moduleObjectPtr = context.getRuntime()->callChimeObjectInvoke(moduleClassPtr, cStringPtr, std::vector<llvm::Value*>());
        
        context.getCurrentScope()->setSelfObjectPtr(moduleObjectPtr);
        
        for (it = this->children()->begin(); it < this->children()->end(); ++it)
        {
            (*it)->codegen(context);
        }
        
        // here, we can destory the self object
        
        return NULL;
    }
}
