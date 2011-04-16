#include "integer_literal.h"
#include "../../parser/parser.h"
#include <assert.h>
#include <sstream>

#include "llvm/DerivedTypes.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/IRBuilder.h"

namespace ast
{
    integer_literal::integer_literal()
    {
        this->value(0);
    }
    
    integer_literal::integer_literal(chime::parser* parser)
    {
        chime::token* t;
        
        assert(parser->look_ahead()->is_integer());
        
        t = parser->next_token();
        this->value(t->integer_value());
        
        delete t;
    }
    
    std::string integer_literal::node_name(void)
    {
        return std::string("integer literal");
    }
    std::string integer_literal::to_string(void)
    {
        std::stringstream s;
        
        s << "literal: ";
        s << this->value();
        
        return s.str();
    }
    
    long integer_literal::value(void) const
    {
        return _value;
    }
    void integer_literal::value(long v)
    {
        _value = v;
    }
    
    llvm::Value* integer_literal::codegen(chime::code_generator& generator)
    {
        llvm::APInt ap_int;
        
        // 64-bit signed value
        ap_int = llvm::APInt(64, this->value(), true);
        
        return llvm::ConstantInt::get(llvm::getGlobalContext(), ap_int);
    }
}
