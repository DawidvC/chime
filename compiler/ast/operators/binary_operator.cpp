#include "binary_operator.h"
#include "compiler/ast/primary/method_call.h"
#include "compiler/ast/literals/literal.h"
#include "IndexOperator.h"

namespace ast
{
    ast::node* binary_operator::parse(chime::parser& parser, int precedence, ast::node* leftOperand)
    {
        ast::binary_operator* node;
        ast::node*            rightOperand;
        chime::token*         t;
        int                   currentPrecedence;
        
        while (true)
        {
            t = parser.look_ahead();
            if (t->empty())
                return leftOperand;
            
            if (t->is_ending())
            {
                return leftOperand;
            }
            
            // we have to store this, because t is just a look-ahead token,
            // and may be removed by future parsing operations
            currentPrecedence = t->precedence();
            
            if (currentPrecedence < precedence)
                return leftOperand;
            
            node = new ast::binary_operator();
            node->identifier(parser.next_token_value());
            
            if (node->identifier().compare(".") == 0)
                rightOperand = new ast::method_call(&parser);
            else
                rightOperand = ast::binary_operator::parseRightOperand(parser);
            
            if (currentPrecedence < parser.look_ahead()->precedence())
            {
                rightOperand = ast::binary_operator::parse(parser, currentPrecedence+1, rightOperand);
            }
            
            node->left_operand(leftOperand);
            node->right_operand(rightOperand);
            leftOperand = node;
            
            // this needs to be done in a loop, to account for
            // multiple levels
            while (parser.look_ahead(1)->equal_to("["))
            {
                leftOperand = ast::IndexOperator::parse(parser, leftOperand);
            }
        }
        
        return leftOperand;
    }
    
    ast::node* binary_operator::parseRightOperand(chime::parser& parser)
    {
        chime::token* t;
        ast::node*    node;
        
        node = NULL;
        
        t = parser.look_ahead();
        if (t == NULL || t->empty())
        {
            chime::parse_error* e;
            
            e = new chime::parse_error("Unable to extract the next token while parsing a primary expression");
            
            parser.add_error(e);
            
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
            node = ast::Literal::parse(parser);
        }
        else if (t->is_type())
        {
            node = parser.parse_type();
        }
        else if (t->isIdentifier())
        {
            node = ast::Variable::parse(parser);
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
    
    ast::node* binary_operator::getRightOperand() const
    {
        return this->childAtIndex(1);
    }
    void binary_operator::setRightOperand(ast::node* op)
    {
        assert(this->childCount() == 1);
        
        this->addChild(op);
    }
    ast::node* binary_operator::right_operand(void) const
    {
        return this->getRightOperand();
    }
    void binary_operator::right_operand(ast::node* op)
    {
        this->setRightOperand(op);
    }
    ast::node* binary_operator::getLeftOperand() const
    {
        return this->childAtIndex(0);
    }
    void binary_operator::setLeftOperand(ast::node* op)
    {
        assert(this->childCount() == 0);
        
        this->addChild(op);
    }
    ast::node* binary_operator::left_operand(void) const
    {
        return this->getLeftOperand();
    }
    void binary_operator::left_operand(ast::node* op)
    {
        this->setLeftOperand(op);
    }
    
    llvm::Value* binary_operator::codegen(chime::code_generator& generator)
    {
        llvm::Value*              l_value;
        llvm::Value*              r_value;
        llvm::Value*              methodNamePtr;
        llvm::LoadInst*           object_load;
        std::vector<llvm::Value*> arguments;
        
        // operators are complex, because the runtime functions that are invoked
        // depend on both the kinds of operator and operands
        if (this->identifier().compare(".") == 0)
        {
            ast::method_call* call;
            
            call = dynamic_cast<ast::method_call*>(this->right_operand());
            assert(call);
            
            l_value = this->left_operand()->codegen(generator);
            assert(l_value);
            
            return call->codegen_with_target(l_value, generator);
        }
        
        // this is just a plain operator that we need to invoke as a method
        l_value = this->left_operand()->codegen(generator);
        r_value = this->right_operand()->codegen(generator);
        assert(l_value && r_value);
        
        object_load = generator.builder()->CreateLoad(r_value, "loaded r_value");
        
        arguments.push_back(object_load);
        
        methodNamePtr = generator.make_constant_string(this->identifier());
        
        return generator.getRuntime()->callChimeObjectInvoke(l_value, methodNamePtr, arguments);
    }    
}
