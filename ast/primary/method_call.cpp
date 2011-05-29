#include "method_call.h"
#include "parser/parser.h"
#include "block.h"
#include "operations/code_generator.h"

namespace ast
{
    method_call::method_call(chime::parser* parser)
    {
        this->identifier(parser->next_token_value());
        
        // ( arguments )
        this->parse_arguments(parser);
    }
    
    std::string method_call::node_name(void)
    {
        return std::string("method call");
    }
    std::string method_call::to_string(void)
    {
        std::string s;
        
        s += "method call: " + this->identifier();
        
        return s;
    }
    
    std::string method_call::identifier() const
    {
        return _identifier;
    }
    void method_call::identifier(std::string s)
    {
        _identifier = s;
    }
    
    void method_call::parse_arguments(chime::parser* parser)
    {
        chime::token* t;
        ast::node*    node;
        
        parser->next_token_value("(");
        
        while (true)
        {
            t = parser->look_ahead();
            
            if (t->empty() || t->equal_to(")"))
                break;
            
            // handle labels
            if (parser->look_ahead(2)->equal_to(":"))
            {
                parser->next_token();
                parser->next_token_value(":");
                if (parser->look_ahead()->equal_to(")"))
                {
                    //there needs to be a block after this
                    break;
                }
            }
            
            node = parser->parse_expression();
            if (node == NULL)
                break;
                
            this->add_child(node);
            
            // take care of the commas
            if (parser->look_ahead()->equal_to(","))
                parser->next_token_value(",");
        }
        
        parser->next_token_value(")");
        
        if (parser->look_ahead()->equal_to("do"))
        {
            this->add_child(new ast::block(parser));
        }
    }
    
    llvm::Value* method_call::codegen_with_target(llvm::Value* target, chime::code_generator& generator)
    {
        llvm::Value*      argument_value;
        llvm::LoadInst*   object_load;
        
        std::vector<llvm::Value*>         arguments;
        std::vector<ast::node*>::iterator i;
        
        for (i = this->children()->begin(); i < this->children()->end(); i++)
        {
            argument_value = (*i)->codegen(generator);
            assert(argument_value);
            
            object_load = generator.builder()->CreateLoad(argument_value, "loaded argument");
            
            arguments.push_back(object_load);
        }
        
        return generator.call_chime_object_invoke(target, this->identifier(), arguments);
    }
}
