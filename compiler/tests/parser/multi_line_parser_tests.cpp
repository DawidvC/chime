#include "parser_tests_base.h"

class MultiLineParserTest : public ParserTestsBase
{
};

TEST_F(MultiLineParserTest, MakeVariableAndUseIt)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("a = Number.new()\na.to_string()");
    
    op = (ast::binary_operator*)node->child_at_index(0);
    assert_operator("=", op);
    assert_entity("a", op->left_operand());
    
    op = (ast::binary_operator*)op->right_operand();
    assert_operator(".", op);
    assert_type("Number", op->left_operand());
    assert_method_call("new", op->right_operand());
    
    op = (ast::binary_operator*)node->child_at_index(1);
    assert_operator(".", op);
    assert_entity("a", op->left_operand());
    assert_method_call("to_string", op->right_operand());
}

TEST_F(MultiLineParserTest, Implemenation)
{
    ast::implementation* node;
    
    node = parse_implementation("implementation SomeClass\n{\n}\n");
    
    assert_implementation("SomeClass", NULL, node);
}

TEST_F(MultiLineParserTest, SimpleMethodDefinition)
{
    ast::method_definition* method;
    
    method = (ast::method_definition*)parse("method new()\n{\n}\n")->child_at_index(0);
    
    assert_method_definition("new", method);
}
