#include "PropertyDefinition.h"
#ifndef USING_PREFIX_HEADERS
#    include "compiler/parser/parser.h"
#    include "compiler/codegen/code_generator.h"
#endif

namespace ast
{
    PropertyDefinition::PropertyDefinition(chime::parser& parser)
    {
        chime::token* t;
        
        // "property identifier"
        parser.next_token_value("property");
        _identifier = parser.next_token_value();
        
        t = parser.look_ahead();
        if (t->equal_to("("))
        {
            _parameters = ParameterSetRef(new ParameterSet(parser));
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
            
            _getBodyBlock = CodeBlockRef(new CodeBlock(parser));
            
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
            
            _setBodyBlock = CodeBlockRef(new CodeBlock(parser));
            
            parser.advance_past_ending_tokens();
            parser.next_token_value("}");
            parser.advance_past_ending_tokens();
        }
        
        parser.advance_past_ending_tokens();
        parser.next_token_value("}");
        parser.advance_past_ending_tokens();
    }
    
    std::string PropertyDefinition::nodeName() const
    {
        return std::string("property definition");
    }
    
    std::string PropertyDefinition::stringRepresentation(int depth) const
    {
        return std::string("unimplemented");
    }
    
    std::string PropertyDefinition::getIdentifier() const
    {
        return _identifier;
    }
    
    CodeBlockRef PropertyDefinition::getGetBody() const
    {
        return _getBodyBlock;
    }
    
    CodeBlockRef PropertyDefinition::getSetBody() const
    {
        return _setBodyBlock;
    }
    
    ParameterSetRef PropertyDefinition::getParameters() const
    {
        return _parameters;
    }
    
    llvm::Value* PropertyDefinition::codegen(chime::code_generator& generator)
    {
        return NULL;
    }
}
