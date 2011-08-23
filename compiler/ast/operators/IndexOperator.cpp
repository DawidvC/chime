#include "IndexOperator.h"

namespace ast
{
    ast::IndexOperator* IndexOperator::parse(chime::parser& parser, ast::node* operand)
    {
        ast::IndexOperator* indexOp;
        ast::NodeRef        argument;
        
        parser.next_token_value("[");
        
        argument = ast::NodeRef(parser.parse_expression());
        assert(argument);
        
        indexOp = new IndexOperator();
        indexOp->setOperand(ast::NodeRef(operand));
        indexOp->setArgument(argument);
        
        parser.next_token_value("]");
        
        return indexOp;
    }
    
    std::string IndexOperator::nodeName(void) const
    {
        return std::string("Index Operator");
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
        
        methodNamePtr = generator.make_constant_string("[]");
        
        return generator.getRuntime()->callChimeObjectInvoke(targetValue, methodNamePtr, arguments);
    }
}
