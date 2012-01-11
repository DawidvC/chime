#include "Root.h"
#include "compiler/ast/variable/GlobalVariable.h"
#include "compiler/ast/variable/SharedLocalVariable.h"

namespace ast
{
    Root::Root()
    {
        _identifier = "Root";
    }
    
    std::string Root::nodeName() const
    {
        return "Root";
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
    
    chime::Variable* Root::createVariable(const std::string& identifier)
    {
        // this is kind of a hack, and duplication with LocalScopedNode.  But, without
        // thinking too hard about it, I think there is no difference between a post-captured
        // local variable and a post-captured global.
        if (this->capturedIdentifier(identifier))
        {
           return new chime::SharedLocalVariable(identifier);
        }
        
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
        
        cStringPtr      = context.getConstantString("Module");
        moduleClassPtr  = context.getRuntime()->callChimeRuntimeGetClass(cStringPtr);
        
        cStringPtr      = context.getConstantString(this->getIdentifier());
        classPtr        = context.getRuntime()->callChimeRuntimeCreateClass(cStringPtr, moduleClassPtr);
        
        this->setClassObjectPtr(classPtr);
        
        cStringPtr      = context.getConstantString("new");
        moduleObjectPtr = context.getRuntime()->callChimeObjectInvoke(classPtr, cStringPtr, std::vector<llvm::Value*>());
        
        this->setSelfObjectPtr(moduleObjectPtr);
        
        context.pushScope(this);
        
        for (it = this->children()->begin(); it < this->children()->end(); ++it)
        {
            (*it)->codegen(context);
        }
        
        // restore scope and the builder's position
        context.popScope();
        
        return NULL;
    }
}
