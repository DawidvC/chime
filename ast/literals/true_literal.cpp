#include "true_literal.h"
#include "parser/parser.h"
#include "operations/code_generator.h"

namespace ast
{
    true_literal::true_literal(chime::parser* parser)
    {
        parser->next_token_value("true");
    }
    
    std::string true_literal::node_name(void)
    {
        return std::string("true literal");
    }
    std::string true_literal::to_string(void)
    {
        std::string s;
        
        s += "literal: true";
        
        return s;
    }
    
    llvm::Value* true_literal::codegen(chime::code_generator& generator)
    {
        return generator.call_chime_literal_encode_boolean(1);
    }
}
