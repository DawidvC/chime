#include "PropertyDefinition.h"
#include "compiler/ast/variable/LocalVariable.h"
#include "compiler/parser/parser.h"
#include "compiler/codegen/code_generator.h"

namespace ast
{
    PropertyDefinition* PropertyDefinition::parse(chime::parser& parser)
    {
        chime::token*       t;
        PropertyDefinition* property;
        
        property = new PropertyDefinition();
        
        parser.pushScope(property);
        
        // "property identifier"
        parser.next_token_value("property");
        property->setIdentifier(parser.next_token_value());
        
        t = parser.look_ahead();
        if (t->equal_to("("))
        {
            property->setParameters(chime::ParameterSet::parse(parser));
        }
        else
        {
            
        }
        
        parser.advance_past_ending_tokens();
        parser.next_token_value("{");
        parser.advance_past_ending_tokens();
        
        if (parser.advanceTokenIfEqual("get"))
        {
            parser.advance_past_ending_tokens();
            parser.next_token_value("{");
            parser.advance_past_ending_tokens();
            
            property->_getBodyBlock = std::tr1::static_pointer_cast<CodeBlock>(CodeBlock::parse(parser));
            
            parser.advance_past_ending_tokens();
            parser.next_token_value("}");
            parser.advance_past_ending_tokens();
        }
        
        if (parser.advanceTokenIfEqual("set"))
        {
            parser.advance_past_ending_tokens();
            parser.next_token_value("{");
            parser.advance_past_ending_tokens();
            
            property->_setBodyBlock = std::tr1::static_pointer_cast<CodeBlock>(CodeBlock::parse(parser));
            
            parser.advance_past_ending_tokens();
            parser.next_token_value("}");
            parser.advance_past_ending_tokens();
        }
        
        if (!property->_getBodyBlock && !property->_setBodyBlock)
        {
            parser.addError("A property must define at least a get or set submethod");
        }
        
        parser.advance_past_ending_tokens();
        parser.next_token_value("}");
        parser.advance_past_ending_tokens();
        
        parser.popScope();
        
        return property;
    }
    
    std::string PropertyDefinition::nodeName() const
    {
        return std::string("property definition");
    }
    
    std::string PropertyDefinition::stringRepresentation(int depth) const
    {
        std::string str;
        
        str.append(depth*2, ' ');
        str.append("property: ");
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
    
    CodeBlockRef PropertyDefinition::getGetBody() const
    {
        return _getBodyBlock;
    }
    
    CodeBlockRef PropertyDefinition::getSetBody() const
    {
        return _setBodyBlock;
    }
    
    chime::Variable* PropertyDefinition::createVariable(const std::string& identifier)
    {
        return new LocalVariable(identifier);
    }
    
    llvm::Value* PropertyDefinition::codegen(chime::code_generator& generator)
    {
        CodeBlockRef body;
        
        body = this->getGetBody();
        if (body)
        {
            this->createMethod(generator, this->getIdentifier(), body, 0);
        }
        
        body = this->getSetBody();
        if (body)
        {
            this->createMethod(generator, this->getIdentifier() + "=", body, 1);
        }
        
        return NULL;
    }
}
