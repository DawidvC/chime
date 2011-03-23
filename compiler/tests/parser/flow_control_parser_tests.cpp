#include "parser_tests_base.h"

class FlowControlParserTest : public ParserTestsBase
{
};

TEST_F(FlowControlParserTest, BlockWithNext)
{
    ast::method_call* call;
    
    call = (ast::method_call*)parse("call() do (a) { next }")->child_at_index(0);
    
    assert_method_call("call", call);
    assert_block(call->child_at_index(0));
    assert_next(call->child_at_index(0)->child_at_index(0));
}

TEST_F(FlowControlParserTest, TailingIf)
{
    ast::if_statement* node;
    
    node = (ast::if_statement*)parse("a = b if c")->child_at_index(0);
    
    assert_if_statement(node);
    assert_entity("c", node->condition());
    assert_operator("=", node->body());
}