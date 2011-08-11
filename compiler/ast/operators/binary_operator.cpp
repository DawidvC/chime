#include "binary_operator.h"
#include "compiler/ast/primary/method_call.h"
#include "compiler/ast/primary/entity_reference.h"
#include "compiler/ast/literals/literal.h"

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
            return new ast::entity_reference(&parser);
        }
        else
        {
            assert(0 && "how did we get to this point");
        }
        
        return node;
    }
    
    binary_operator::binary_operator()
    {
    }
    
    std::string binary_operator::node_name(void) const
    {
        return std::string("binary operator");
    }
    std::string binary_operator::to_string(void) const
    {
        std::string s;
        
        s += "operator: " + this->identifier();
        
        return s;
    }
    
    ast::node* binary_operator::right_operand(void) const
    {
        return this->child_at_index(1);
    }
    void binary_operator::right_operand(ast::node* op)
    {
        assert(this->child_count() == 1);
        
        this->add_child(op);
    }
    ast::node* binary_operator::left_operand(void) const
    {
        return this->child_at_index(0);
    }
    void binary_operator::left_operand(ast::node* op)
    {
        assert(this->child_count() == 0);
        
        this->add_child(op);
    }
    
    llvm::Value* binary_operator::codegen(chime::code_generator& generator)
    {
        llvm::Value*              l_value;
        llvm::Value*              r_value;
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
            
            return call->codegen_with_target(l_value, generator);;
        }
        else if (this->identifier().compare("=") == 0)
        {
            return this->codegen_assignment(generator);
        }
        
        // this is just a plain operator that we need to invoke as a method
        l_value = this->left_operand()->codegen(generator);
        r_value = this->right_operand()->codegen(generator);
        assert(l_value && r_value);
        
        object_load = generator.builder()->CreateLoad(r_value, "loaded r_value");
        
        arguments.push_back(object_load);
        
        return generator.call_chime_object_invoke(l_value, this->identifier(), arguments);
    }
    
    llvm::Value* binary_operator::codegen_assignment(chime::code_generator& generator)
    {
        ast::entity_reference* entity;
        llvm::Value*           lValue;
        llvm::Value*           rValue;
        llvm::LoadInst*        objectLoad;
        
        // assignments can happen only to entity references
        entity = dynamic_cast<ast::entity_reference*>(this->left_operand());
        
        rValue = this->right_operand()->codegen(generator);
        
        // first, is it an instance variable?
        if (generator.isEntityAnInstanceVariable(entity->identifier()))
        {
            llvm::Value* self;
            llvm::Value* attributeNameCStringPtr;
            
            attributeNameCStringPtr = generator.make_constant_string(entity->identifier());
            
            self = generator.getMethodScope()->getSelfPointer();
            
            generator.getRuntime()->callChimeObjectSetAttribute(self, attributeNameCStringPtr, rValue);
            
            fprintf(stderr, "Assinging to ivar %s\n", entity->identifier().c_str());
            
            // the return of this statement isn't super obvious
            return rValue;
        }
        
        objectLoad = generator.builder()->CreateLoad(rValue, "loaded r_value");
        
        // this is just a regular old local variable
        lValue = this->left_operand()->codegen(generator);
        assert(lValue);
        
        generator.builder()->CreateStore(objectLoad, lValue, false);
        
        return lValue;
    }
}
