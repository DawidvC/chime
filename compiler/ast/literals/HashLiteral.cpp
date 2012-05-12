#include "HashLiteral.h"

namespace chime
{
    Node* HashLiteral::parse(parser& parser)
    {
        HashLiteral* hash;
        
        parser.nextTokenValue("{");
        
        hash = new HashLiteral();
        
        do
        {
            // the key
            hash->addChild(parser.parse_expression());
            
            parser.nextTokenValue(":");
            
            // the value
            hash->addChild(parser.parse_expression());
            
            if (parser.advanceTokenIfEqual("}"))
            {
                break;
            }
            else if (parser.lookAhead()->isStatementEnding())
            {
                parser.addError("Found a statement ending token in a hash literal");
                break;
            }
            
        } while (parser.advanceTokenIfEqual(","));
        
        return hash;
    }
    
    std::string HashLiteral::nodeName() const
    {
        return "Hash Literal";
    }
    
    llvm::Value* HashLiteral::codegen(CodeGenContext& context)
    {
        llvm::Value*                 hashValue;
        std::vector<Node*>::iterator it;
        
        // there must be an even number of children
        assert(this->childCount() % 2 == 0);
        
        hashValue = context.getRuntime()->callChimeHashCreate();
        
        for (it = this->children()->begin(); it != this->children()->end(); ++it)
        {
            llvm::Value* keyValue;
            llvm::Value* valueValue;
            
            keyValue = (*it)->codegen(context);
            
            ++it;
            
            valueValue = (*it)->codegen(context);
            
            context.getRuntime()->callChimeHashSet(hashValue, keyValue, valueValue);
        }
        
        return hashValue;
    }
}
