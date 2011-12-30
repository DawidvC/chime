#include "IndexerDefinition.h"

namespace chime
{
    IndexerDefinition* IndexerDefinition::parse(parser& parser)
    {
        IndexerDefinition*        indexer;
        std::vector<ParameterRef> parameters;
        
        indexer = new IndexerDefinition();
        
        parser.pushScope(indexer);
        
        // "property identifier"
        parser.nextTokenValue("indexer");
        if (parser.advanceTokenIfEqual("self"))
        {
            indexer->setIdentifier("[]:");
        }
        else
        {
            indexer->setIdentifier(parser.nextTokenValue() + "[]:");
        }
        
        parser.nextTokenValue("[");
        parameters.push_back(Parameter::parse(parser));
        parser.nextTokenValue("]");
        
        if (parser.advanceTokenIfEqual("("))
        {
            parameters.push_back(Parameter::parse(parser));
            parser.nextTokenValue(")");
        }
        
        indexer->setParameters(parameters);
        
        parser.advanceToNextStatement();
        parser.nextTokenValue("{");
        parser.advanceToNextStatement();
        
        if (parser.advanceTokenIfEqual("get"))
        {
            parser.advanceToNextStatement();
            parser.nextTokenValue("{");
            parser.advanceToNextStatement();
            
            indexer->_getBodyBlock = static_pointer_cast<CodeBlock>(CodeBlock::parse(parser));
            
            parser.advanceToNextStatement();
            parser.nextTokenValue("}");
            parser.advanceToNextStatement();
        }
        
        if (parser.advanceTokenIfEqual("set"))
        {
            parser.advanceToNextStatement();
            parser.nextTokenValue("{");
            parser.advanceToNextStatement();
            
            indexer->_setBodyBlock = static_pointer_cast<CodeBlock>(CodeBlock::parse(parser));
            
            parser.advanceToNextStatement();
            parser.nextTokenValue("}");
            parser.advanceToNextStatement();
        }
        
        parser.advanceToNextStatement();
        parser.nextTokenValue("}");
        parser.advanceToNextStatement();
        
        parser.popScope();
        
        return indexer;
    }
    
    std::string IndexerDefinition::nodeName() const
    {
        return "Indexer Definition";
    }
    
    std::string IndexerDefinition::stringRepresentation(int depth) const
    {
        std::string str;
        
        str.append(depth*2, ' ');
        str.append("indexer: ");
        str.append(this->getIdentifier());
        
        if (this->getGetBody())
        {
            str.append("\n");
            str.append((depth+1)*2, ' ');
            str.append("- get:\n");
            str.append(this->getGetBody()->stringRepresentation(depth+2));
        }
        
        if (this->getSetBody())
        {
            str.append("\n");
            str.append((depth+1)*2, ' ');
            str.append("- set:\n");
            str.append(this->getSetBody()->stringRepresentation(depth+2));
        }
        
        return str;
    }
    
    llvm::Value* IndexerDefinition::codegen(CodeGenContext& context)
    {
        chime::CodeBlockRef body;
        
        body = this->getGetBody();
        if (body)
        {
            this->createMethod(context, this->getIdentifier(), body, 1);
        }
        
        body = this->getSetBody();
        if (body)
        {
            this->createMethod(context, this->getIdentifier() + "=:", body, 2);
        }
        
        return NULL;
    }
}
