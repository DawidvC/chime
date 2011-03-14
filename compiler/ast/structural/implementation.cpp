#include "implementation.h"

namespace ast
{
    implementation::implementation(chime::parser* parser)
    {
        // parse the import statement
        parser->next_token_value("implementation");
    }
    
    std::string implementation::node_name(void)
    {
        return std::string("implementation");
    }
}
