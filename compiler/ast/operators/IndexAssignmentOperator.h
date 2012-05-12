// chime: IndexAssignmentOperator.h

#ifndef INDEX_ASSIGNMENT_OPERATOR_H
#define INDEX_ASSIGNMENT_OPERATOR_H

#include "IndexOperator.h"

namespace chime
{
    class IndexAssignmentOperator : public IndexOperator
    {
    public:
        std::string nodeName() const;
        std::string stringRepresentation(int depth=0) const;
        
        NodeRef      getRightOperand() const;
        void         setRightOperand(NodeRef node);
        
        llvm::Value* codegen(CodeGenContext& context);
        
    private:
        NodeRef _rightOperand;
    };
}

#endif // INDEX_ASSIGNMENT_OPERATOR_H
