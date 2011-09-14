#include "Variable.h"
#include "compiler/ast/operators/binary_operator.h"
#include "compiler/ast/operators/IndexOperator.h"

namespace ast
{
    node* Variable::parse(chime::parser& parser, bool allowAssignment)
    {
        std::string identifier;
        node*       node;
        
        assert(parser.look_ahead()->isIdentifier());
        
        identifier = parser.next_token_value();
        
        // Defer to the current scope to actually create the variable object.  This is
        // the trick to make scope-aware variables in the AST
        node = parser.getCurrentScope()->variableForIdentifier(identifier);
        assert(node);
        
        if (allowAssignment && parser.look_ahead(1)->value() == "=")
        {
            AssignmentOperator* op;
            
            parser.next_token_value("="); // parse the "="
            
            op = static_cast<Variable*>(node)->createAssignment();
            
            op->setLeftOperand(node);
            op->setRightOperand(parser.parse_expression());
            
            return op;
        }
        
        // check for index operators
        while (parser.look_ahead(1)->equal_to("["))
        {
            node = ast::IndexOperator::parse(parser, node);
        }
        
        return node;
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
