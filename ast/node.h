// node.h
//

#ifndef AST_NODE
#define AST_NODE

#include <vector>
#include <string>
#include <assert.h>
#include <tr1/memory>

using std::tr1::shared_ptr;

#include "llvm/DerivedTypes.h"

namespace chime
{
    class parser;
    
    class code_generator;
}

namespace ast
{
    class node
    {
    public:
        node();
        virtual ~node();
        
        void                     add_child(ast::node* new_child);
        std::vector<ast::node*>* children(void) const;
        ast::node*               child_at_index(unsigned int i) const;
        void                     replace_child_at_index(unsigned int i, ast::node* new_child);
        unsigned int             child_count() const;
        
        virtual std::string      node_name(void);
        virtual std::string      nodeName(void) const;
        virtual std::string      to_string(void);
        virtual std::string      stringRepresentation(int depth=0);
        void                     print(void);
        
        virtual llvm::Value*     codegen(chime::code_generator& generator);
        
    protected:
        std::vector<ast::node*>* _children;
    };
}
#endif
