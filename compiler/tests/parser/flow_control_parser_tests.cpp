#include "parser_tests_base.h"

class FlowControlParserTest : public ParserTestsBase
{
};

TEST_F(FlowControlParserTest, ClosureWithNext)
{
    ast::MethodCall* call;
    ast::Closure*     closure;
    
    call = parseMethodCall("call() do (a) { next }");
    
    ASSERT_METHOD_CALL("call", call);
    
    closure = static_cast<ast::Closure*>(call->childAtIndex(0));
    ASSERT_CLOSURE(closure);
    ASSERT_NEXT(closure->getBody()->childAtIndex(0));
}

TEST_F(FlowControlParserTest, IfWithNoElse)
{
    ast::IfStatement* node;
    
    node = (ast::IfStatement*)parse("if true foo()")->child_at_index(0);
    
    ASSERT_IF_STATEMENT(node);
    ASSERT_LITERAL_TRUE(node->getCondition().get());
    ASSERT_METHOD_CALL("foo", node->getBody().get());
    ASSERT_TRUE(node->getElse() == NULL);
}

TEST_F(FlowControlParserTest, IfWithElse)
{
    ast::IfStatement* node;
    
    node = (ast::IfStatement*)parse("if true foo() else bar()")->child_at_index(0);
    
    ASSERT_IF_STATEMENT(node);
    ASSERT_LITERAL_TRUE(node->getCondition().get());
    ASSERT_METHOD_CALL("foo", node->getBody().get());
    ASSERT_METHOD_CALL("bar", node->getElse().get());
}

TEST_F(FlowControlParserTest, IfWithElseOnDifferentLines)
{
    ast::IfStatement* node;
    
    node = (ast::IfStatement*)parse("if true\n foo()\n else\n bar()")->child_at_index(0);
    
    ASSERT_IF_STATEMENT(node);
    ASSERT_LITERAL_TRUE(node->getCondition().get());
    ASSERT_METHOD_CALL("foo", node->getBody().get());
    ASSERT_METHOD_CALL("bar", node->getElse().get());
}

TEST_F(FlowControlParserTest, IfWithBracesAndNoElse)
{
    ast::IfStatement* node;
    
    node = (ast::IfStatement*)parse("if true { foo() }")->child_at_index(0);
    
    ASSERT_IF_STATEMENT(node);
    ASSERT_LITERAL_TRUE(node->getCondition().get());
    
    ASSERT_CODE_BLOCK(node->getBody().get());
    ASSERT_METHOD_CALL("foo", ((ast::CodeBlock*)node->getBody().get())->childAtIndex(0));
    
    ASSERT_TRUE(node->getElse() == NULL);
}

TEST_F(FlowControlParserTest, IfWithElseAndBracesOnNewLines)
{
    ast::IfStatement* node;
    
    node = (ast::IfStatement*)parse("if true\n{\n foo()\n}\nelse\n{\nbar()\n }")->child_at_index(0);
    
    ASSERT_IF_STATEMENT(node);
    ASSERT_LITERAL_TRUE(node->getCondition().get());
    
    ASSERT_CODE_BLOCK(node->getBody().get());
    ASSERT_METHOD_CALL("foo", ((ast::CodeBlock*)node->getBody().get())->childAtIndex(0));
    
    ASSERT_CODE_BLOCK(node->getElse().get());
    ASSERT_METHOD_CALL("bar", ((ast::CodeBlock*)node->getElse().get())->childAtIndex(0));
}

TEST_F(FlowControlParserTest, IfWithNestedIf)
{
    ast::IfStatement* node;
    
    node = (ast::IfStatement*)parse("if true if false foo()")->child_at_index(0);
    
    ASSERT_IF_STATEMENT(node);
    ASSERT_LITERAL_TRUE(node->getCondition().get());
    ASSERT_IF_STATEMENT(node->getBody().get());
}

TEST_F(FlowControlParserTest, TailingIf)
{
    ast::IfStatement* node;
    
    node = static_cast<ast::IfStatement*>(parse("a = b if c")->childAtIndex(0));
    
    ASSERT_IF_STATEMENT(node);
    ASSERT_GLOBAL_VARIABLE("c", node->getCondition().get());
    ASSERT_GLOBAL_ASSIGNMENT(node->getBody().get());
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

TEST_F(FlowControlParserTest, ThrowStatement)
{
    ast::Throw* node;
    
    node = (ast::Throw*)parse("throw \"boom\"")->child_at_index(0);
    
    ASSERT_THROW_STATEMENT(node);
    ASSERT_LITERAL_STRING("boom", node->child_at_index(0));
}

TEST_F(FlowControlParserTest, ReturnInIfStatement)
{
    ast::IfStatement* node;
    
    node = (ast::IfStatement*)parse("if false\n  return")->child_at_index(0);
    
    ASSERT_IF_STATEMENT(node);
    ASSERT_RETURN(node->getBody().get());
    ASSERT_TRUE(node->getElse() == NULL);
}

TEST_F(FlowControlParserTest, ReturnVoidStatment)
{
    ast::method_definition* method;
    
    method = parse_method_def("method foo() { return }");
    
    ASSERT_METHOD_DEFINITION("foo", method);
    ASSERT_RETURN(method->getBody()->childAtIndex(0));
}

TEST_F(FlowControlParserTest, SwitchStatement)
{
    ast::Switch* switchNode;
    ast::CaseRef caseNode;
    
    switchNode = static_cast<ast::Switch*>(parse("switch (a) {\n case 1\n foo()\nelse\n bar()\n}")->childAtIndex(0));
    
    ASSERT_GLOBAL_VARIABLE("a", switchNode->getExpression().get());
    ASSERT_EQ(1, switchNode->getCases().size());
    
    caseNode = switchNode->getCases()[0];
    ASSERT_LITERAL_INTEGER(1, caseNode->getCondition().get());
    ASSERT_METHOD_CALL("foo", caseNode->getBody().get());
    
    ASSERT_METHOD_CALL("bar", switchNode->getElse().get());
}

TEST_F(FlowControlParserTest, SwitchStatementWithNoElse)
{
    ast::Switch* switchNode;
    ast::CaseRef caseNode;
    
    switchNode = static_cast<ast::Switch*>(parse("switch (a) {\n case 1\n foo()\n}")->childAtIndex(0));
    
    ASSERT_GLOBAL_VARIABLE("a", switchNode->getExpression().get());
    ASSERT_EQ(1, switchNode->getCases().size());
    
    caseNode = switchNode->getCases()[0];
    ASSERT_LITERAL_INTEGER(1, caseNode->getCondition().get());
    ASSERT_METHOD_CALL("foo", caseNode->getBody().get());
    
    ASSERT_FALSE(switchNode->getElse());
}
