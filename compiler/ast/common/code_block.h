// chime: code_block.h

#ifndef CODE_BLOCK_H
#define CODE_BLOCK_H

#include "compiler/ast/node.h"
#include "compiler/parser/LexicalScope.h"

namespace ast
{
    class CodeBlock : public node
    {
    public:
        static NodeRef parseNextBlock(chime::parser& parser, bool allowStructural=false);
        
    public:
        CodeBlock(chime::parser& parser, bool allowStructural=false);
        
        virtual std::string  nodeName() const;
        virtual std::string  stringRepresentation(int depth=0) const;
        
        void                   addChild(const ast::node& node);
        ast::node*             childAtIndex(unsigned int i) const;
        chime::LexicalScopeRef getScope() const;
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
    
    private:
        chime::LexicalScopeRef _scope;
    };
    
    typedef std::tr1::shared_ptr<ast::CodeBlock> CodeBlockRef;
}
#endif // CODE_BLOCK_H
