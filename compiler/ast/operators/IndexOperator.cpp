#include "IndexOperator.h"
#include "IndexAssignmentOperator.h"

namespace ast
{
    ast::node* IndexOperator::parse(chime::parser& parser, ast::node* operand, bool allowAssignment)
    {
        ast::IndexOperator* indexOp;
        ast::NodeRef        argument;
        
        // check for index operators
        while (parser.advanceTokenIfEqual("["))
        {
            argument = parser.parseExpression();
            assert(argument);
            
            parser.nextTokenValue("]");
            
            if (allowAssignment && parser.lookAhead()->isAssignment())
            {
                chime::IndexAssignmentOperator* assignment;
                
                parser.nextTokenValue("=");
                
                assignment = new chime::IndexAssignmentOperator();
                assignment->setIdentifier("[]:=:");
                assignment->setOperand(ast::NodeRef(operand));
                assignment->setArgument(argument);
                assignment->setRightOperand(parser.parseExpression());
                
                return assignment;
            }
            
            indexOp = new IndexOperator();
            indexOp->setOperand(ast::NodeRef(operand));
            indexOp->setArgument(argument);
            
            operand = indexOp;
        }
        
        return operand;
    }
    
    std::string IndexOperator::nodeName(void) const
    {
        return std::string("Index Operator");
    }
    
    std::string IndexOperator::stringRepresentation(int depth) const
    {
        std::string str;
        
        str.append(depth*2, ' ');
        str.append(this->nodeName());
        str.append("\n");
        
        str.append(this->getArgument()->stringRepresentation(depth+1));
        
        str.append("\n");
        
        str.append(this->getOperand()->stringRepresentation(depth+1));
        
        return str;
    }
    
    ast::NodeRef IndexOperator::getOperand(void) const
    {
        return _operand;
    }
    
    void IndexOperator::setOperand(ast::NodeRef node)
    {
        _operand = node;
    }
    
    ast::NodeRef IndexOperator::getArgument(void) const
    {
        return _argument;
    }
    
    void IndexOperator::setArgument(ast::NodeRef node)
    {
        _argument = node;
    }
    
    llvm::Value* IndexOperator::codegen(chime::code_generator& generator)
    {
        llvm::Value*              argumentValue;
        llvm::Value*              targetValue;
        llvm::LoadInst*           objectLoad;
        llvm::Value*              methodNamePtr;
        std::vector<llvm::Value*> arguments;
        
        argumentValue = this->getArgument()->codegen(generator);
        assert(argumentValue);
        
        targetValue = this->getOperand()->codegen(generator);
        
        objectLoad = generator.builder()->CreateLoad(argumentValue, "loaded argument");
        
        arguments.push_back(objectLoad);
        
        methodNamePtr = generator.make_constant_string("[]:");
        
        return generator.getRuntime()->callChimeObjectInvoke(targetValue, methodNamePtr, arguments);
    }
}
