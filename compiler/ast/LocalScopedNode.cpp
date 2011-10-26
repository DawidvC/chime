#include "LocalScopedNode.h"
#include "compiler/ast/variable/LocalVariable.h"
#include "compiler/ast/variable/SharedLocalVariable.h"

namespace chime
{
    std::string LocalScopedNode::getIdentifier() const
    {
        return std::string("LocalScopedNode");
    }
    
    ast::Variable* LocalScopedNode::createVariable(const std::string& identifier)
    {
        // if this variable has been captured, at this point in the state of
        // parsing, then we need to do something special
        if (this->capturedIdentifier(identifier))
        {
           return new ast::SharedLocalVariable(identifier);
        }
        
        return new ast::LocalVariable(identifier);
    }
}