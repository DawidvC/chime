#include "Variable.h"
#include "compiler/ast/operators/binary_operator.h"
#include "compiler/ast/operators/IndexOperator.h"

namespace ast
{
    Node* Variable::parse(chime::parser& parser, bool allowAssignment)
    {
        std::string identifier;
        node*       node;
        
        assert(parser.look_ahead()->isIdentifier());
        
        identifier = parser.next_token_value();
        
        // Defer to the current scope to actually create the variable object.  This is
        // the trick to make scope-aware variables in the AST
        node = parser.getCurrentScope()->variableForIdentifier(identifier);
        assert(node);
        
        if (allowAssignment && parser.lookAhead()->isAssignment())
        {
            return Variable::parseAssignment(parser, static_cast<Variable*>(node));
        }
        
        // check for index operators
        node = ast::IndexOperator::parse(parser, node);
        
        return node;
    }
    
    Node* Variable::parseAssignment(chime::parser& parser, Variable* variable)
    {
        AssignmentOperator* op;
        BinaryOperator*     binOp;
        
        op = variable->createAssignment();
        op->setLeftOperand(variable);
        
        if (parser.lookAhead()->equal_to("="))
        {
            parser.nextTokenValue("="); // parse the "="
            
            op->setRightOperand(parser.parse_expression());
            
            return op;
        }
        
        binOp = new ast::BinaryOperator();
        
        if (parser.lookAhead()->equal_to("+="))
        {
            parser.nextTokenValue("+=");
            
            binOp->setIdentifier("+");
        }
        else if (parser.lookAhead()->equal_to("-="))
        {
            parser.nextTokenValue("-=");
            
            binOp->setIdentifier("-");
        }
        else if (parser.lookAhead()->equal_to("*="))
        {
            parser.nextTokenValue("*=");
            
            binOp->setIdentifier("*");
        }
        else if (parser.lookAhead()->equal_to("/="))
        {
            parser.nextTokenValue("/=");
            
            binOp->setIdentifier("/");
        }
        else
        {
            parser.addError("What kind of compound assignment operator is you?");
        }
        
        binOp->setLeftOperand(variable);
        binOp->setRightOperand(parser.parse_expression());
        
        op->setRightOperand(binOp);
        
        return op;
    }
    
    Variable::Variable(const std::string& identifier)
    {
        _identifier = identifier;
    }
    
    std::string Variable::stringRepresentation(int depth) const
    {
        std::string s;
        
        s.append(depth*2, ' ');
        
        s.append(this->nodeName());
        s.append(": ");
        s.append(this->getIdentifier());
        
        return s;
    }
    
    std::string Variable::getIdentifier() const
    {
        return _identifier;
    }
    
    llvm::Value* Variable::codegenReference(chime::code_generator& generator)
    {
        llvm::Value* variableValue;
        llvm::Value* referenceValue;
        
        variableValue  = this->codegen(generator);
        referenceValue = generator.getRuntime()->callChimeReferenceCreate(variableValue);
        
        return referenceValue;
    }
}
