#ifndef CODE_CONTAINER
#define CODE_CONTAINER

#include <string>
#include "../node.h"
#include "method_parameter.h"

namespace ast
{
    class code_container : public node
    {
    public:
        code_container(void);
        virtual ~code_container();
        
        void                            add_parameter(method_parameter* param);
        std::vector<method_parameter*>* parameters(void) const;
        ast::method_parameter*          parameter_at_index(unsigned int index) const;
        
    protected:
        std::vector<method_parameter*>* _parameters;
        
        void parse_parameters(chime::parser* parser);
        void parse_body(chime::parser* parser, bool with_structural);
    };
}

#endif
