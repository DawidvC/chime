// chime: import.h

#ifndef IMPORT_NODE_H
#define IMPORT_NODE_H

#include "compiler/ast/node.h"

namespace ast
{
    class Import : public node
    {
    public:
        Import(chime::parser& parser);
        virtual ~Import();
        
        virtual std::string nodeName(void) const;
        
        ast::node*   getImportand() const;
        void         setImportand(ast::node* n);
        
        llvm::Value* codegen(chime::code_generator& generator);
    };
}

#endif // IMPORT_NODE_H
