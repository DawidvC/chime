// chime: import.h

#ifndef IMPORT_NODE_H
#define IMPORT_NODE_H

#include "compiler/ast/node.h"

namespace chime
{
    class Import : public node
    {
    public:
        Import(chime::parser& parser);
        virtual ~Import();
        
        virtual std::string nodeName(void) const;
        
        Node* getImportand() const;
        void  setImportand(Node* n);
        
        llvm::Value* codegen(chime::code_generator& generator);
    };
}

#endif // IMPORT_NODE_H
