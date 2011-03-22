#include "parser_tests_base.h"

class MultiLineParserTest : public ParserTestsBase
{
};

TEST_F(MultiLineParserTest, MakeVariableAndUseIt)
{
    ast::node* node;
    
    node = parse("a = Number.new()\na.to_string()");
    
    node->print();
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
