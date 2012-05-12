#ifndef STRING_LITERAL_H
#define STRING_LITERAL_H

#include "Literal.h"

namespace chime
{
    class string_literal : public Literal
    {
    public:
        static Node* parse(chime::parser& parser);
    
    public:
        virtual std::string nodeName(void) const;
        std::string stringRepresentation(int depth=0) const;
        
        std::string value(void) const;
        void value(std::string v);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
        
    private:
        std::string _value;
    };
}

#endif // STRING_LITERAL_H
