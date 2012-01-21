#include "parser_tests_base.h"
#include "compiler/lexer/file_lexer.h"

class FileParserTests : public ParserTestsBase
{
    virtual chime::lexer* lex(const char* input)
    {
        chime::lexer* lexer;
        std::string   s;
        
        s += "/tmp/chime/compiler/tests/fixtures/";
        s += input;
        
        lexer = new chime::filelexer(s.c_str());
        
        return lexer;
    }
};

TEST_F(FileParserTests, BasicImplemenation)
{
    ast::node* node;
    
    node = this->parse("basic_implementation.cm");
    
    ASSERT_IMPLEMENTATION("Foo", "Bar", node->child_at_index(0));
    ASSERT_EQ(1, (int)node->child_count());
}

TEST_F(FileParserTests, BasicMethodDefinition)
{
    ast::method_definition* method;
    
    method = this->parse_method_def("basic_method_definition.cm");
    
    ASSERT_METHOD_DEFINITION("new", method);
}

TEST_F(FileParserTests, StoreToAVariableAndUseIt)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = this->parse("store_then_use.cm");
    
    op = static_cast<ast::binary_operator*>(node->childAtIndex(0));
    ASSERT_GLOBAL_ASSIGNMENT(op);
    ASSERT_GLOBAL_VARIABLE("a", op->getLeftOperand());
    
    op = static_cast<ast::binary_operator*>(op->getRightOperand());
    ASSERT_OPERATOR(".", op);
    ASSERT_TYPE("Object", op->getLeftOperand());
    ASSERT_METHOD_CALL("new", op->getRightOperand());
    
    op = static_cast<ast::binary_operator*>(node->childAtIndex(1));
    ASSERT_OPERATOR(".", op);
    ASSERT_GLOBAL_VARIABLE("a", op->getLeftOperand());
    ASSERT_METHOD_CALL("to_string", op->getRightOperand());
}

TEST_F(FileParserTests, TwoClassMethodsInARow)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("two_class_methods_in_a_row.cm");
    
    op = (ast::binary_operator*)node->child_at_index(0);
    ASSERT_OPERATOR(".", op);
    ASSERT_TYPE("Foo", op->left_operand());
    ASSERT_METHOD_CALL("bar", op->right_operand());
    
    op = (ast::binary_operator*)node->child_at_index(1);
    ASSERT_OPERATOR(".", op);
    ASSERT_TYPE("Shoo", op->left_operand());
    ASSERT_METHOD_CALL("baz", op->right_operand());
}
