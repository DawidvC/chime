#include "method_definition.h"
#include "../../parser/parser.h"

namespace ast
{
    method_definition::method_definition(chime::parser* parser)
    {
        _parameters = new std::vector<method_parameter*>();
        
        // method identifier
        parser->next_token_value("method");
        this->identifier(parser->next_token_value());
        
        // ( parameters )
        parser->next_token_value("(");
        this->parse_parameters(parser);
        parser->next_token_value(")");
        
        parser->advance_past_ending_tokens();
        
        // { body }
        parser->next_token_value("{");
        
        parser->advance_past_ending_tokens();
        
        this->parse_body(parser);
        
        parser->advance_past_ending_tokens();
        
        parser->next_token_value("}");
        
        parser->advance_past_ending_tokens();
    }
    
    method_definition::~method_definition()
    {
        delete _parameters;
    }
    
    std::string method_definition::node_name(void)
    {
        return std::string("method definition");
    }
    std::string method_definition::to_string(void)
    {
        std::string s;
        
        s += "method: " + this->identifier();
        
        return s;
    }
    
    std::string method_definition::identifier() const
    {
        return _identifier;
    }
    void method_definition::identifier(std::string s)
    {
        _identifier = s;
    }
    
    void method_definition::add_parameter(method_parameter* param)
    {
        _parameters->push_back(param);
    }
    std::vector<method_parameter*>* method_definition::parameters(void) const
    {
        return _parameters;
    }
    ast::method_parameter* method_definition::parameter_at_index(unsigned int index) const
    {
        return (*_parameters)[index];
    }
    
    void method_definition::parse_parameters(chime::parser* parser)
    {
        chime::token* t;
        
        while (true)
        {
            t = parser->look_ahead();
            
            if (t->empty() || t->equal_to(")"))
                break;
            
            this->add_parameter(new method_parameter(parser));
            
            // take care of the commas
            if (parser->look_ahead()->equal_to(","))
                parser->next_token_value(",");
        }
    }
    
    void method_definition::parse_body(chime::parser* parser)
    {
        ast::node* node;
        
        while (true)
        {
            // we've reached the end of the body
            if (parser->look_ahead()->equal_to("}"))
                break;
                
            node = parser->parse_without_structural();
            if (!node)
            {
                chime::parse_error* e;
                
                e = new chime::parse_error("method_definition::parse_body: unable to get next node");
                
                parser->errors()->push_back(e);
                break;
            }
            
            this->add_child(node);
        }
    }
}