// chime: IndexerDefinition.h

#ifndef INDEXER_DEFINITON_H
#define INDEXER_DEFINITON_H

#include "PropertyDefinition.h"
#include "compiler/ast/CodeBlock.h"

namespace chime
{
    class IndexerDefinition : public ast::PropertyDefinition
    {
    public:
        static IndexerDefinition* parse(parser& parser);
    
    public:
        virtual std::string nodeName() const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        llvm::Value* codegen(CodeGenContext& context);
    };
}

#endif // INDEXER_DEFINITON_H
