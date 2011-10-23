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
        return NULL;
    }
}
