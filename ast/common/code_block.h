// chime: code_block.h

#ifndef CODE_BLOCK_H
#define CODE_BLOCK_H

#include "ast/node.h"

namespace ast
{
    class CodeBlock : public node
    {
    public:
        CodeBlock(chime::parser& parser);
        virtual ~CodeBlock();
        
        virtual std::string  nodeName(void) const;
        virtual std::string  stringRepresentation(int depth=0) const;
        
        void                 addChild(const ast::node& node);
        ast::node*           childAtIndex(unsigned int i) const;
        
        static shared_ptr<ast::node> nextBlock(chime::parser& parser);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
    };
}
#endif // CODE_BLOCK_H
