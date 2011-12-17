#include "Variable.h"
#include "compiler/ast/operators/binary_operator.h"
#include "compiler/ast/operators/IndexOperator.h"

namespace chime
{
    Node* Variable::parse(parser& parser, bool allowAssignment)
    {
        std::string identifier;
        Node*       node;
        
        assert(parser.look_ahead()->isIdentifier());
        
        identifier = parser.nextTokenValue();
        
        // Defer to the current scope to actually create the variable object.  This is
        // the trick to make scope-aware variables in the AST
        node = parser.getCurrentScope()->variableForIdentifier(identifier);
        assert(node);
        
        if (allowAssignment && parser.lookAhead()->isAssignment())
        {
            return Variable::parseAssignment(parser, static_cast<Variable*>(node));
        }
        
        // check for index operators
        node = ast::IndexOperator::parse(parser, node, allowAssignment);
        
        return node;
    }
    
    Node* Variable::parseAssignment(parser& parser, Variable* variable)
    {
        ast::AssignmentOperator* op;
        BinaryOperator*          binOp;
        
        op = variable->createAssignment();
        op->setLeftOperand(variable);
        
        if (parser.advanceTokenIfEqual("="))
        {
            op->setRightOperand(parser.parse_expression());
            
            return op;
        }
        
        binOp = new ast::BinaryOperator();
        
        if (parser.advanceTokenIfEqual("+="))
        {
            binOp->setIdentifier("+");
        }
        else if (parser.advanceTokenIfEqual("-="))
        {
            binOp->setIdentifier("-");
        }
        else if (parser.advanceTokenIfEqual("*="))
        {
            binOp->setIdentifier("*");
        }
        else if (parser.advanceTokenIfEqual("/="))
        {
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
        _defined    = false;
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
        return this->identifier();
    }
    
    std::string Variable::identifier() const
    {
        return _identifier;
    }
    
    // all of these are totally un-OO
    bool Variable::isVariable() const
    {
        return true;
    }
    
    bool Variable::requiresRelease() const
    {
        return true;
    }
    
    bool Variable::requiresCapture() const
    {
        return false;
    }
    
    bool Variable::isDefined() const
    {
        return _defined;
    }
    
    void Variable::setDefined(bool value)
    {
        _defined = value;
    }
    
    Variable* Variable::createClosedVersion()
    {
        return this;
    }
    
    llvm::Value* Variable::codegenReference(chime::code_generator& generator)
    {
        llvm::Value* variableValue;
        llvm::Value* referenceValue;
        
        variableValue  = this->codegen(generator);
        referenceValue = generator.getRuntime()->callChimeReferenceCreate(variableValue);
        
        // this is actually an ownership transfer, so we need to release the variable
        generator.getRuntime()->callChimeObjectRelease(variableValue);
        
        return referenceValue;
    }
}
