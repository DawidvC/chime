#include "parser_tests_base.h"

class FlowControlParserTest : public ParserTestsBase
{
};

TEST_F(FlowControlParserTest, BlockWithNext)
{
    ast::method_call* call;
    
    call = (ast::method_call*)parse("call() do (a) { next }")->child_at_index(0);
    
    ASSERT_METHOD_CALL("call", call);
    ASSERT_BLOCK(call->child_at_index(0));
    assert_next(call->child_at_index(0)->child_at_index(0));
}

TEST_F(FlowControlParserTest, TailingIf)
{
    ast::if_statement* node;
    
    node = (ast::if_statement*)parse("a = b if c")->child_at_index(0);
    
    assert_if_statement(node);
    ASSERT_ENTITY("c", node->condition());
    ASSERT_OPERATOR("=", node->body());
}

TEST_F(FlowControlParserTest, BasicTryCatch)
{
    ast::node* node;
    
    node = parse("try { foo() } catch (e) { bar() }")->child_at_index(0);
    
    ASSERT_TRY(node);
    ASSERT_METHOD_CALL("foo", node->child_at_index(0));
    
    node = ((ast::Try*)node)->getCatchBlocks()->at(0);
    ASSERT_CATCH(node);
    ASSERT_METHOD_CALL("bar", node->child_at_index(0)); 
}

TEST_F(FlowControlParserTest, BasicTryCatchFinally)
{
    ast::Try* node;
    
    node = (ast::Try*)parse("try { foo() } catch (e) { bar() } finally { baz() }")->child_at_index(0);
    
    ASSERT_TRY(node);
    ASSERT_CATCH(node->getCatchBlocks()->at(0));
    ASSERT_FINALLY(node->getFinallyBlock());
    ASSERT_METHOD_CALL("baz", node->getFinallyBlock()->child_at_index(0)); 
}

TEST_F(FlowControlParserTest, BasicTryFinally)
{
    ast::Try* node;
    
    node = (ast::Try*)parse("try { foo() } finally { bar() }")->child_at_index(0);
    
    ASSERT_TRY(node);
    ASSERT_FINALLY(node->getFinallyBlock());
    ASSERT_EQ(0, node->getCatchBlocks()->size());
}
