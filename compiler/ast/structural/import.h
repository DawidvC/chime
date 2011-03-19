#include <string>
#include "../node.h"
#include "../primary/type_reference.h"

#ifndef IMPORT_NODE
#define IMPORT_NODE

namespace ast
{
    class import : public node
    {
    public:
        import(chime::parser* parser);
        virtual ~import();
        
        virtual std::string node_name(void);
        virtual std::string to_string(void);
        
        ast::type_reference* importand() const;
        void                 importand(ast::type_reference* n);
    
    protected:
        ast::type_reference* _importand;
    };
}

#endif
