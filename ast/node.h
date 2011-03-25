// node.h
//

#ifndef AST_NODE
#define AST_NODE

#include <vector>

#include "llvm/DerivedTypes.h"
// #include "llvm/LLVMContext.h"
// #include "llvm/Module.h"
// #include "llvm/Analysis/Verifier.h"
// #include "llvm/Support/IRBuilder.h"

namespace chime
{
    class parser;
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
        unsigned int             child_count() const;
        
        virtual std::string      node_name(void);
        virtual std::string      to_string(void);
        virtual std::string      string_representation(int depth=0);
        void                     print(void);
        
        virtual llvm::Value*     codegen(void);
        
    protected:
        std::vector<ast::node*>* _children;
    };
}
#endif
