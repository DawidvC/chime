#include "ArrayLiteral.h"

namespace chime
{
    ast::Node* ArrayLiteral::parse(parser& parser)
    {
        ArrayLiteral* array;
        
        parser.nextTokenValue("[");
        
        array = new ArrayLiteral();
        
        do
        {
            array->addChild(parser.parse_expression());
            
            if (parser.advanceTokenIfEqual("]"))
            {
                break;
            }
            else if (parser.lookAhead()->isStatementEnding())
            {
                parser.addError("Found a statement ending token in a array literal");
                break;
            }
            
        } while (parser.advanceTokenIfEqual(","));
        
        return array;
    }
    
    std::string ArrayLiteral::nodeName() const
    {
        return "Array Literal";
    }
    
    llvm::Value* ArrayLiteral::codegen(CodeGenContext& context)
    {
        llvm::Value*                      arrayValue;
        llvm::Value*                      sizeValue;
        std::vector<ast::Node*>::iterator it;
        
        sizeValue = llvm::ConstantInt::get(context.getContext(), llvm::APInt(64, this->childCount(), 10));
        
        arrayValue = context.getRuntime()->callChimeArrayCreateWithLength(sizeValue);
        
        for (it = this->children()->begin(); it != this->children()->end(); ++it)
        {
            llvm::Value* elementValue;
            
            elementValue = (*it)->codegen(context);
            
            context.getRuntime()->callChimeArrayAppend(arrayValue, elementValue);
        }
        
        return arrayValue;
    }
}
