#include "IndexerDefinition.h"

namespace chime
{
    IndexerDefinition* IndexerDefinition::parse(parser& parser)
    {
        IndexerDefinition* indexer;
        ParameterSetRef    parameters;
        
        indexer = new IndexerDefinition();
        
        parser.pushScope(indexer);
        
        // "property identifier"
        parser.nextTokenValue("indexer");
        if (parser.advanceTokenIfEqual("self"))
        {
            indexer->setIdentifier("[]");
        }
        else
        {
            indexer->setIdentifier(parser.nextTokenValue() + "[]");
        }
        
        parameters = ParameterSetRef(new ParameterSet());
        
        parser.nextTokenValue("[");
        parameters->addChild(new ast::method_parameter(&parser));
        parser.nextTokenValue("]");
        
        if (parser.advanceTokenIfEqual("("))
        {
            parameters->addChild(new ast::method_parameter(&parser));
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
            
            indexer->_getBodyBlock = static_pointer_cast<ast::CodeBlock>(ast::CodeBlock::parse(parser));
            
            parser.advanceToNextStatement();
            parser.nextTokenValue("}");
            parser.advanceToNextStatement();
        }
        
        if (parser.advanceTokenIfEqual("set"))
        {
            parser.advanceToNextStatement();
            parser.nextTokenValue("{");
            parser.advanceToNextStatement();
            
            indexer->_setBodyBlock = static_pointer_cast<ast::CodeBlock>(ast::CodeBlock::parse(parser));
            
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
        return "unimplemented";
    }
    
    llvm::Value* IndexerDefinition::codegen(CodeGenContext& context)
    {
        ast::CodeBlockRef body;
        
        body = this->getGetBody();
        if (body)
        {
            this->createMethod(context, this->getIdentifier(), body, 1);
        }
        
        body = this->getSetBody();
        if (body)
        {
            this->createMethod(context, this->getIdentifier() + "=", body, 2);
        }
        
        return NULL;
    }
}
