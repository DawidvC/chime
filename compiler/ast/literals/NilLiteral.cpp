#include "NilLiteral.h"

namespace chime
{
    std::string NilLiteral::nodeName() const
    {
        return "Nil Literal";
    }
    
    std::string NilLiteral::stringRepresentation(int depth) const
    {
        std::string s;
        
        s.append(depth*2, ' ');
        s.append(this->nodeName());
        
        return s;
    }
    
    llvm::Value* NilLiteral::codegen(CodeGenContext& context)
    {
        llvm::AllocaInst* alloca;
        
        alloca = context.insertChimeObjectAlloca();
        
        context.builder()->CreateStore(context.getRuntime()->getChimeLiteralNull(), alloca, false);
        
        return alloca;
    }
}
