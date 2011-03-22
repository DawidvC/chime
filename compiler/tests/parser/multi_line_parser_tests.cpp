#include "parser_tests_base.h"

class MultiLineParserTest : public ParserTestsBase
{
};

TEST_F(MultiLineParserTest, MakeVariableAndUseIt)
{
    ast::node* node;
    
    //node = parse("a = Number.new()");
    node = parse("a = Number.new()\na.to_string()");
    
    node->print();
}