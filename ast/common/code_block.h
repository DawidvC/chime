// chime: code_block.h

#ifndef CODE_BLOCK_H
#define CODE_BLOCK_H

#include "ast/node.h"

namespace ast
{
    class CodeBlock : public node
    {
    public:
        CodeBlock(chime::parser& parser, bool allowStructural=false);
        virtual ~CodeBlock();
        
        virtual std::string  nodeName(void) const;
        virtual std::string  stringRepresentation(int depth=0) const;
        
        void                 addChild(const ast::node& node);
        ast::node*           childAtIndex(unsigned int i) const;
        
        static NodeRef       nextBlock(chime::parser& parser);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
    };
    
    typedef shared_ptr<ast::CodeBlock> CodeBlockRef;
}
#endif // CODE_BLOCK_H
