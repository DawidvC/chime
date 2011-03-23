#include "code_container.h"
#include "../../parser/parser.h"

namespace ast
{
    code_container::code_container(void)
    {
        _parameters = new std::vector<method_parameter*>();
    }
    
    code_container::~code_container()
    {
        delete _parameters;
    }
    
    void code_container::add_parameter(method_parameter* param)
    {
        _parameters->push_back(param);
    }
    std::vector<method_parameter*>* code_container::parameters(void) const
    {
        return _parameters;
    }
    ast::method_parameter* code_container::parameter_at_index(unsigned int index) const
    {
        return (*_parameters)[index];
    }
    
    void code_container::parse_parameters(chime::parser* parser)
    {
        chime::token* t;
        
        parser->next_token_value("(");
        
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
        
        parser->next_token_value(")");
    }
    
    void code_container::parse_body(chime::parser* parser)
    {
        ast::node* node;
        
        parser->advance_past_ending_tokens();
        parser->next_token_value("{");
        parser->advance_past_ending_tokens();
        
        while (true)
        {
            // we've reached the end of the body
            if (parser->look_ahead()->equal_to("}"))
                break;
                
            node = parser->parse_without_structural();
            if (!node)
            {
                chime::parse_error* e;
                
                e = new chime::parse_error("code_container::parse_body: unable to get next node");
                
                parser->errors()->push_back(e);
                break;
            }
            
            this->add_child(node);
        }
        
        parser->advance_past_ending_tokens();
        parser->next_token_value("}");
        parser->advance_past_ending_tokens();
    }
}