#include <string>
#include "../node.h"
#include "method_parameter.h"

#ifndef METHOD_DEFINITON
#define METHOD_DEFINITON

namespace ast
{
    class method_definition : public node
    {
    public:
        method_definition(chime::parser* parser);
        virtual ~method_definition();
        
        virtual std::string node_name(void);
        virtual std::string to_string(void);
        
        std::string         identifier() const;
        void                identifier(std::string s);
        
        void                            add_parameter(method_parameter* param);
        std::vector<method_parameter*>* parameters(void) const;
        ast::method_parameter*          parameter_at_index(unsigned int index) const;
        
    protected:
        std::string                     _identifier;
        std::vector<method_parameter*>* _parameters;
        
        void parse_parameters(chime::parser* parser);
        void parse_body(chime::parser* parser);
    };
}

#endif
