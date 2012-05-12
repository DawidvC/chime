// chime: ScopedNode.h

#ifndef SCOPED_NODE_H
#define SCOPED_NODE_H

#include "compiler/ast/node.h"
#include "compiler/ast/literals/SelfLiteral.h"
#include "compiler/ast/variable/Variable.h"

#include <map>

namespace chime
{
    class ScopedNode : public node
    {
    public:
        ScopedNode();
        
        virtual std::string getIdentifier() const = 0;
        ScopedNode* parent() const;
        void        setParent(ScopedNode* parent);
        ScopedNode* ancestorThatDefinesSelf() const;
        ScopedNode* ancestorThatDefinesClassObject() const;
        ScopedNode* enclosingImplementation() const;
        void        setEnclosingImplementation(ScopedNode* implemenation);
        
        bool                     definedIdentifier(const std::string& identifier) const;
        void                     defineIdentifier(const std::string& identifier);
        bool                     capturedIdentifier(const std::string& identifier);
        
        virtual chime::Variable* createVariable(const std::string& identifier);
        virtual chime::Variable* transformVariable(chime::Variable* variable);
        void                     capturedVariable(chime::Variable* variable);
        chime::Variable*         variableForIdentifier(const std::string& identifier);
        
        virtual chime::SelfLiteral* createSelf();
        chime::SelfLiteral*         getSelf();
        
        virtual bool         allowsStructuralElements() const;
        virtual bool         isFunction() const;
        
        // codegen support
        virtual void         addLooseValue(llvm::Value* value);
        virtual void         removeLooseValue(llvm::Value* value);
        virtual llvm::Value* getValueForIdentifier(const std::string& identifier);
        virtual void         setValueForIdentifier(const std::string& identifier, llvm::Value* value, bool shouldRelease=true);
        virtual llvm::Value* selfValue(chime::CodeGenContext& context);
        virtual llvm::Value* selfObjectPtr() const;
        virtual void         setSelfObjectPtr(llvm::Value* value);
        virtual llvm::Value* classObjectPtr() const;
        virtual void         setClassObjectPtr(llvm::Value* value);
        
        void                 codegenScopeExit(chime::CodeGenContext& context, std::vector<std::string> identifiersToSkip=std::vector<std::string>());
        void                 codegenFunctionExit(chime::CodeGenContext& context, std::vector<std::string> identifiersToSkip=std::vector<std::string>());
        
        std::map<std::string, llvm::Value*> scopedValues() const;
        
        int               incrementAnonymousFunctionCount();
        
        llvm::BasicBlock* getStartBlock() const;
        void              setStartBlock(llvm::BasicBlock* block);
        llvm::BasicBlock* getEndBlock() const;
        void              setEndBlock(llvm::BasicBlock* block);
        
    private:
        std::vector<std::string>                _variableNames;
        std::map<std::string, chime::Variable*> _capturedVariables;
        ScopedNode*                             _parent;
        ScopedNode*                             _implementation;
        
        std::map<std::string, llvm::Value*> _scopedValues;
        std::vector<std::string>            _scopedValuesToSkip;
        std::vector<llvm::Value*>           _looseValues;
        llvm::Value*                        _selfObjectPtr;
        llvm::Value*                        _classObjectPtr;
        int                                 _anonymousFunctionCount;
        
        llvm::BasicBlock* _startBlock;
        llvm::BasicBlock* _endBlock;
    };
    
    typedef std::tr1::shared_ptr<ScopedNode> ScopedNodeRef;
}

#endif // SCOPED_NODE_H
