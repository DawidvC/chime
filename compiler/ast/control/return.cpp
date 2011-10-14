#include "return.h"

namespace ast
{
    Return::Return(chime::parser& parser)
    {
        chime::Token* t;
        
        parser.next_token("return");
        
        // we need to deal with an argument to return here, but there could be other things
        t = parser.look_ahead();
        if (t->isStatementEnding() || t->isControl())
            return;
        
        _returnValue = parser.parseExpression();
    }
    
    std::string Return::nodeName(void) const
    {
        return std::string("return");
    }
    
    std::string Return::stringRepresentation(int depth) const
    {
        std::string str;
        
        str.append(depth*2, ' ');
        str.append(this->nodeName());
        
        if (this->getReturnValue())
        {
            str.append("\n");
            str.append(this->getReturnValue()->stringRepresentation(depth+1));
        }
        
        return str;
    }
    
    ast::NodeRef Return::getReturnValue() const
    {
        return _returnValue;
    }
    
    llvm::Value* Return::codegen(chime::code_generator& generator)
    {
        llvm::Value* value;
        
        // if the block has already been terminated, skip this
        if (generator.builder()->GetInsertBlock()->getTerminator())
            return NULL;
        
        if (this->getReturnValue())
        {
            // codegen the return value
            value = this->getReturnValue()->codegen(generator);
            
            // load it, so we can return the result
            value = generator.builder()->CreateLoad(value, "return value load");
        }
        else
        {
            value = generator.getRuntime()->getChimeLiteralNull();
        }
        
        generator.builder()->CreateRet(value);
        
        return NULL;
    }
}
