// node.h

#ifndef AST_NODE
#define AST_NODE

#include "compiler/shared_ptr.h"

#include <llvm/Value.h>

#include <string>
#include <vector>

namespace chime
{
    class parser;
    
    class code_generator;
    typedef code_generator CodeGenContext;;
}

namespace chime
{
    class node
    {
    public:
        node();
        virtual ~node();
        
        void                       addChild(chime::node* n);
        void                       add_child(chime::node* new_child);
        std::vector<chime::node*>* children(void) const;
        chime::node*               childAtIndex(unsigned int i) const;
        chime::node*               child_at_index(unsigned int i) const;
        void                       replace_child_at_index(unsigned int i, chime::node* new_child);
        unsigned int               childCount() const;
        unsigned int               child_count() const;
        
        virtual std::string  node_name(void) const;
        virtual std::string  nodeName(void) const;
        virtual std::string  to_string(void) const;
        virtual std::string  stringRepresentation(int depth=0) const;
        void                 print(void);
        
        virtual bool         isVariable() const;
        virtual bool         isSelf() const;
        virtual bool         requiresRelease() const;
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
        
    protected:
        std::vector<chime::node*>* _children;
    };
    
    typedef shared_ptr<chime::node> NodeRef;
    typedef chime::node Node;
}

#endif
