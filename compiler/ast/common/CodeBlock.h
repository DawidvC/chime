// chime: CodeBlock.h

#ifndef CODE_BLOCK_H
#define CODE_BLOCK_H

#include "compiler/ast/ScopedNode.h"

namespace ast
{
    class CodeBlock : public ScopedNode
    {
    public:
        static NodeRef parse(chime::parser& parser, bool allowStructural=false);
        static NodeRef parseBlockWithOptionalBraces(chime::parser& parser);
        
    public:
        virtual std::string nodeName() const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        std::string getIdentifier() const;
        
        void                addChild(const ast::node& node);
        ast::node*          childAtIndex(unsigned int i) const;
        
        llvm::Value* selfValue(chime::CodeGenContext& context);
        llvm::Value* selfObjectPtr() const;
        void         setSelfObjectPtr(llvm::Value* value);
        llvm::Value* classObjectPtr() const;
        void         setClassObjectPtr(llvm::Value* value);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
    };
    
    typedef std::tr1::shared_ptr<ast::CodeBlock> CodeBlockRef;
}
#endif // CODE_BLOCK_H
