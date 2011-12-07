// chime: CodeBlock.h

#ifndef CODE_BLOCK_H
#define CODE_BLOCK_H

#include "compiler/ast/ScopedNode.h"

namespace chime
{
    class CodeBlock : public ast::ScopedNode
    {
    public:
        static NodeRef parse(chime::parser& parser, bool allowStructural=false);
        static NodeRef parseBlockWithOptionalBraces(chime::parser& parser);
        
    public:
        CodeBlock();
        
        virtual std::string nodeName() const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        std::string getIdentifier() const;
        
        bool        deferToParent() const;
        void        setDeferToParent(bool value);
        
        void         addChild(const ast::node& node);
        ast::node*   childAtIndex(unsigned int i) const;
        
        void         addLooseValue(llvm::Value* value);
        void         removeLooseValue(llvm::Value* value);
        llvm::Value* getValueForIdentifier(const std::string& identifier);
        void         setValueForIdentifier(const std::string& identifier, llvm::Value* value, bool shouldRelease=true);
        llvm::Value* selfValue(chime::CodeGenContext& context);
        llvm::Value* selfObjectPtr() const;
        void         setSelfObjectPtr(llvm::Value* value);
        llvm::Value* classObjectPtr() const;
        void         setClassObjectPtr(llvm::Value* value);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
        
    private:
        bool _deferToParent;
    };
    
    typedef shared_ptr<CodeBlock> CodeBlockRef;
}

#endif // CODE_BLOCK_H
