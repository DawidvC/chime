#include "Include.h"

namespace chime
{
    Include* Include::parse(parser& parser)
    {
        Include* include;
        
        include = new Include();
        
        parser.nextTokenValue("include");
        
        include->_traitName = TypeRef(new Type(&parser));
        
        return include;
    }
    
    std::string Include::nodeName() const
    {
        return "Include";
    }
    
    TypeRef Include::getTraitType() const
    {
        return _traitName;
    }
    
    llvm::Value* Include::codegen(CodeGenContext& context)
    {
        llvm::Value* traitObjectPtr;
        llvm::Value* traitNamePtr;
        llvm::Value* classObjectPtr;
        
        assert(context.getCurrentScope()->allowsStructuralElements());
        
        traitNamePtr   = context.getConstantString(this->getTraitType()->identifier());
        traitObjectPtr = context.getRuntime()->callChimeRuntimeGetTrait(traitNamePtr);
        classObjectPtr = context.getCurrentScope()->getSelfValue(context);
        
        context.getRuntime()->callChimeClassIncludeTrait(classObjectPtr, traitObjectPtr);
        
        return NULL;
    }
}
