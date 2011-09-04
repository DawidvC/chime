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
            property->setParameters(ParameterSetRef(new ParameterSet(parser)));
        }
        
        parser.advance_past_ending_tokens();
        parser.next_token_value("{");
        parser.advance_past_ending_tokens();
        
        t = parser.look_ahead();
        if (t->equal_to("get"))
        {
            parser.next_token_value("get");
            parser.advance_past_ending_tokens();
            parser.next_token_value("{");
            parser.advance_past_ending_tokens();
            
            property->_getBodyBlock = CodeBlockRef(new CodeBlock(parser));
            
            parser.advance_past_ending_tokens();
            parser.next_token_value("}");
            parser.advance_past_ending_tokens();
        }
        
        t = parser.look_ahead();
        if (t->equal_to("set"))
        {
            parser.next_token_value("set");
            parser.advance_past_ending_tokens();
            parser.next_token_value("{");
            parser.advance_past_ending_tokens();
            
            property->_setBodyBlock = CodeBlockRef(new CodeBlock(parser));
            
            parser.advance_past_ending_tokens();
            parser.next_token_value("}");
            parser.advance_past_ending_tokens();
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
        return std::string("unimplemented");
    }
    
    CodeBlockRef PropertyDefinition::getGetBody() const
    {
        return _getBodyBlock;
    }
    
    CodeBlockRef PropertyDefinition::getSetBody() const
    {
        return _setBodyBlock;
    }
    
    Variable* PropertyDefinition::createVariable(const std::string& identifier)
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
