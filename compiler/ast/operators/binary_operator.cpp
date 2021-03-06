#include "binary_operator.h"
#include "AndOperator.h"
#include "IndexOperator.h"
#include "MethodCall.h"
#include "OrOperator.h"
#include "compiler/ast/literals/Literal.h"

namespace chime
{
    Node* binary_operator::parse(chime::parser& parser, int precedence, Node* leftOperand)
    {
        binary_operator* node;
        Node*            rightOperand;
        chime::token*    t;
        int              currentPrecedence;
        
        if (leftOperand == NULL)
        {
            parser.addError("Unable to parse left operand of Binary Operator");
            return NULL;
        }
        
        while (true)
        {
            t = parser.look_ahead();
            if (t->empty())
                return leftOperand;
            
            if (t->isStatementEnding())
            {
                return leftOperand;
            }
            
            // we have to store this, because t is just a look-ahead token,
            // and may be removed by future parsing operations
            currentPrecedence = t->precedence();
            
            if (currentPrecedence < precedence)
                return leftOperand;
            
            if (parser.advanceTokenIfEqual("and"))
            {
                node = new chime::AndOperator();
            }
            else if (parser.advanceTokenIfEqual("or"))
            {
                node = new chime::OrOperator();
            }
            else
            {
                node = new BinaryOperator();
                node->identifier(parser.nextTokenValue());
            }
            
            if (node->identifier().compare(".") == 0)
                rightOperand = MethodCall::parse(parser);
            else
            {
                rightOperand = binary_operator::parseRightOperand(parser);
            }
            
            if (currentPrecedence < parser.look_ahead()->precedence())
            {
                rightOperand = binary_operator::parse(parser, currentPrecedence+1, rightOperand);
            }
            
            if (rightOperand == NULL)
            {
                parser.addError("Unable to parse right operand of Binary Operator");
                return leftOperand;
            }
            
            node->setLeftOperand(leftOperand);
            node->setRightOperand(rightOperand);
            leftOperand = node;
            
            leftOperand = IndexOperator::parse(parser, leftOperand);
        }
        
        return leftOperand;
    }
    
    Node* binary_operator::parseRightOperand(chime::parser& parser)
    {
        chime::token* t;
        Node*    node;
        
        node = NULL;
        
        t = parser.look_ahead();
        if (t == NULL || t->empty())
        {
            parser.addError("Unable to extract the next token while parsing a right operand for a binary operator");
            
            return NULL;
        }
        else if (t->equal_to("("))
        {
            parser.next_token_value("(");
            
            node = parser.parse_expression();
            
            parser.next_token_value(")");
        }
        else if (t->isLiteral())
        {
            node = Literal::parse(parser);
        }
        else if (t->isType())
        {
            node = parser.parse_type();
        }
        else if (t->isIdentifier())
        {
            node = chime::Variable::parse(parser);
        }
        else
        {
            assert(0 && "how did we get to this point");
        }
        
        return node;
    }
    
    std::string binary_operator::nodeName(void) const
    {
        return std::string("Binary Operator");
    }
    std::string binary_operator::stringRepresentation(int depth) const
    {
        std::string s;
        
        s.append(depth*2, ' ');
        s.append(this->nodeName());
        s.append(": ");
        s.append(this->identifier());
        
        s.append("\n");
        
        s.append(this->getLeftOperand()->stringRepresentation(depth+1));
        
        s.append("\n");
        
        s.append(this->getRightOperand()->stringRepresentation(depth+1));
        
        return s;
    }
    
    Node* binary_operator::getRightOperand() const
    {
        return this->childAtIndex(1);
    }
    void binary_operator::setRightOperand(Node* op)
    {
        assert(op);
        assert(this->childCount() == 1);
        
        this->addChild(op);
    }
    Node* binary_operator::right_operand(void) const
    {
        return this->getRightOperand();
    }
    void binary_operator::right_operand(Node* op)
    {
        this->setRightOperand(op);
    }
    Node* binary_operator::getLeftOperand() const
    {
        return this->childAtIndex(0);
    }
    void binary_operator::setLeftOperand(Node* op)
    {
        assert(op);
        assert(this->childCount() == 0);
        
        this->addChild(op);
    }
    Node* binary_operator::left_operand(void) const
    {
        return this->getLeftOperand();
    }
    void binary_operator::left_operand(Node* op)
    {
        this->setLeftOperand(op);
    }
    
    llvm::Value* binary_operator::codegen(chime::code_generator& generator)
    {
        llvm::Value*              l_value;
        llvm::Value*              r_value;
        llvm::Value*              methodNamePtr;
        llvm::Value*              result;
        llvm::LoadInst*           object_load;
        std::vector<llvm::Value*> arguments;
        
        // operators are complex, because the runtime functions that are invoked
        // depend on both the kinds of operator and operands
        if (this->identifier().compare(".") == 0)
        {
            MethodCall* call;
            
            call = dynamic_cast<MethodCall*>(this->getRightOperand());
            assert(call);
            
            l_value = this->getLeftOperand()->codegen(generator);
            assert(l_value);
            
            result = call->codegenWithTarget(generator, l_value);
            
            generator.getCurrentScope()->addLooseValue(result);
            
            return result;
        }
        
        // this is just a plain operator that we need to invoke as a method
        l_value = this->getLeftOperand()->codegen(generator);
        r_value = this->getRightOperand()->codegen(generator);
        assert(l_value && r_value);
        
        object_load = generator.builder()->CreateLoad(r_value, "loaded r_value");
        
        arguments.push_back(object_load);
        
        methodNamePtr = generator.getConstantString(this->identifier());
        
        result = generator.getRuntime()->callChimeObjectInvoke(l_value, methodNamePtr, arguments);
        
        generator.getCurrentScope()->addLooseValue(result);
        
        return result;
    }    
}
