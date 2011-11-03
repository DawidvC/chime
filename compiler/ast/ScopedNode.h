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
        ScopedNode* getParent() const;
        void        setParent(ScopedNode* parent);
        
        std::vector<std::string> getContainedVariableNames();
        bool                     definedIdentifier(const std::string& identifier);
        bool                     capturedIdentifier(const std::string& identifier);
        
        virtual Variable* createVariable(const std::string& identifier);
        virtual Variable* transformVariable(Variable* variable);
        void              capturedVariable(Variable* variable);
        Variable*         variableForIdentifier(const std::string& identifier);
        
        virtual chime::SelfLiteral* createSelf();
        chime::SelfLiteral*         getSelf();
        
        virtual bool         allowsStructuralElements() const;
        
        // codegen support
        llvm::Value*         getValueForIdentifier(const std::string& identifier);
        void                 setValueForIdentifier(const std::string& identifier, llvm::Value* value);
        virtual llvm::Value* getSelfValue(chime::CodeGenContext& context);
        llvm::Value*         getSelfObjectPtr() const;
        void                 setSelfObjectPtr(llvm::Value* value);
        llvm::Value*         getClassObjectPtr() const;
        void                 setClassObjectPtr(llvm::Value* value);
        
        std::string       getAnonymousFunctionName();
        
        virtual llvm::BasicBlock* getStartBlock() const;
        virtual llvm::BasicBlock* getEndBlock() const;
        
    protected:
        std::vector<std::string>         _variableNames;
        std::map<std::string, Variable*> _capturedVariables;
        ScopedNode*                      _parent;
        
        std::map<std::string, llvm::Value*> _scopedValues;
        llvm::Value*                        _selfObjectPtr;
        llvm::Value*                        _classObjectPtr;
        int                                 _anonymousFunctionCount;
    };
    
    typedef std::tr1::shared_ptr<ScopedNode> ScopedNodeRef;
}

#endif // SCOPED_NODE_H
