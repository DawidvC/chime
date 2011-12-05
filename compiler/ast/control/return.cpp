#include "return.h"
#include "SingleVariableReturn.h"

namespace chime
{
    Return* Return::parse(parser& parser)
    {
        Token*  t;
        Return* node;
        NodeRef expression;
        
        parser.nextToken("return");
        
        // we need to deal with an argument to return here, but there could be other things
        t = parser.look_ahead();
        if (t->isStatementEnding() || t->isControl())
            return new Return(); // plain return statement
        
        expression = parser.parseExpression();
        
        if (expression->isVariable())
            node = new SingleVariableReturn();
        else
            node = new Return();
        
        node->_returnValue = expression;
        
        return node;
    }
    
    std::string Return::nodeName() const
    {
        return "return";
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
        
        // Return is actually a pretty complex operation.  First, we need to
        // evaluate the return value, if any
        if (this->getReturnValue())
        {
            // codegen the return value
            value = this->getReturnValue()->codegen(generator);
            
            generator.getCurrentScope()->removeLooseValue(value);
            
            // load it, so we can return the result
            value = generator.builder()->CreateLoad(value, "return value load");
        }
        else
        {
            value = generator.getRuntime()->getChimeLiteralNull();
        }
        
        // after getting a reference to our return value, above, we need
        // to clear out all of our scope references, all the way up to
        // the enclosing function
        generator.getCurrentScope()->codegenFunctionExit(generator);
                
        generator.builder()->CreateRet(value);
        
        return NULL;
    }
}
