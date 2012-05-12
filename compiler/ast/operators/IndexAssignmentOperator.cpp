#include "IndexAssignmentOperator.h"

namespace chime
{
    std::string IndexAssignmentOperator::nodeName() const
    {
        return "Index Assignment";
    }
    
    std::string IndexAssignmentOperator::stringRepresentation(int depth) const
    {
        return "unimplemented";
    }
    
    NodeRef IndexAssignmentOperator::getRightOperand() const
    {
        return _rightOperand;
    }
    
    void IndexAssignmentOperator::setRightOperand(NodeRef node)
    {
        _rightOperand = node;
    }
    
    llvm::Value* IndexAssignmentOperator::codegen(CodeGenContext& context)
    {
        llvm::Value*              argumentValue;
        llvm::Value*              targetValue;
        llvm::Value*              rightValue;
        llvm::LoadInst*           indexLoad;
        llvm::LoadInst*           rightLoad;
        llvm::Value*              methodNamePtr;
        std::vector<llvm::Value*> arguments;
        
        argumentValue = this->getArgument()->codegen(context);
        assert(argumentValue);
        
        rightValue = this->getRightOperand()->codegen(context);
        assert(rightValue);
        
        targetValue = this->getOperand()->codegen(context);
        assert(targetValue);
        
        indexLoad = context.builder()->CreateLoad(argumentValue, "[]=: index");
        rightLoad = context.builder()->CreateLoad(rightValue,    "[]=: value");
        
        arguments.push_back(indexLoad);
        arguments.push_back(rightLoad);
        
        methodNamePtr = context.getConstantString(this->getIdentifier());
        
        return context.getRuntime()->callChimeObjectInvoke(targetValue, methodNamePtr, arguments);
    }
}
