// chime: ScopedNode.h

#ifndef SCOPED_NODE_H
#define SCOPED_NODE_H

#include "compiler/ast/node.h"
#include "compiler/ast/literals/SelfLiteral.h"
#include "compiler/ast/variable/Variable.h"

#include <map>

namespace ast
{
    class ScopedNode : public node
    {
    public:
        ScopedNode();
        
        virtual std::string getIdentifier() const = 0;
        ScopedNode* parent() const;
        void        setParent(ScopedNode* parent);
        ScopedNode* enclosingImplementation() const;
        void        setEnclosingImplementation(ScopedNode* implemenation);
        
        bool                     definedIdentifier(const std::string& identifier);
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
        void                 addLooseValue(llvm::Value* value);
        void                 removeLooseValue(llvm::Value* value);
        llvm::Value*         getValueForIdentifier(const std::string& identifier);
        void                 setValueForIdentifier(const std::string& identifier, llvm::Value* value);
        virtual llvm::Value* getSelfValue(chime::CodeGenContext& context);
        llvm::Value*         getSelfObjectPtr() const;
        void                 setSelfObjectPtr(llvm::Value* value);
        llvm::Value*         getClassObjectPtr() const;
        void                 setClassObjectPtr(llvm::Value* value);
        
        void                 codegenScopeExit(chime::CodeGenContext& context, std::vector<std::string> identifiersToSkip=std::vector<std::string>());
        void                 codegenFunctionExit(chime::CodeGenContext& context, std::vector<std::string> identifiersToSkip=std::vector<std::string>());
        
        std::map<std::string, llvm::Value*> scopedValues() const;
        
        std::string       getAnonymousFunctionName();
        
        llvm::BasicBlock* getStartBlock() const;
        void              setStartBlock(llvm::BasicBlock* block);
        llvm::BasicBlock* getEndBlock() const;
        void              setEndBlock(llvm::BasicBlock* block);
        
    protected:
        std::vector<std::string>                _variableNames;
        std::map<std::string, chime::Variable*> _capturedVariables;
        ScopedNode*                             _parent;
        ScopedNode*                             _implementation;
        
        std::map<std::string, llvm::Value*> _scopedValues;
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
