#include "parser_tests_base.h"

class ClosureParseTests : public ParserTestsBase
{
};

TEST_F(ClosureParseTests, MethodCallWithClosure)
{
    ast::MethodCall* call;
    
    call = parseMethodCall("call(a, b) do {}");
    
    ASSERT_METHOD_CALL("call", call);
    ASSERT_GLOBAL_VARIABLE("a", call->childAtIndex(0));
    ASSERT_GLOBAL_VARIABLE("b", call->childAtIndex(1));
    ASSERT_CLOSURE(call->childAtIndex(2));
}

TEST_F(ClosureParseTests, MethodCallWithClosureParameters)
{
    ast::MethodCall* call;
    ast::Closure*     closure;
    
    call = parseMethodCall("call() do (a) { }");
    
    ASSERT_METHOD_CALL("call", call);
    
    closure = static_cast<ast::Closure*>(call->childAtIndex(0));
    ASSERT_CLOSURE(closure);
    
    ASSERT_METHOD_PARAMETER(NULL, NULL, "a", closure->getParameters()->childAtIndex(0));
}

TEST_F(ClosureParseTests, MethodCallWithClosureAndBody)
{
    ast::MethodCall*     call;
    ast::Closure*         closure;
    ast::binary_operator* op;
    
    call = parseMethodCall("call() do { a = \"string\" }");
    
    ASSERT_METHOD_CALL("call", call);
    
    closure = static_cast<ast::Closure*>(call->childAtIndex(0));
    ASSERT_CLOSURE(closure);
    
    op = static_cast<ast::binary_operator*>(closure->getBody()->childAtIndex(0));
    ASSERT_LOCAL_ASSIGNMENT(op);
    ASSERT_LOCAL_VARIABLE("a", op->getLeftOperand());
}

TEST_F(ClosureParseTests, AnonymousFunctionDefinition)
{
    ast::binary_operator* op;
    
    op = parseOperator("f = do { return 5; }");
    
    ASSERT_GLOBAL_ASSIGNMENT(op);
    ASSERT_GLOBAL_VARIABLE("f", op->getLeftOperand());
    ASSERT_CLOSURE(op->getRightOperand());
}

TEST_F(ClosureParseTests, MethodCallWithLabelledClosure)
{
    ast::MethodCall*     call;
    ast::Closure*         closure;
    ast::binary_operator* op;
    
    call = parseMethodCall("on_queue(queue, async:) do { queue = 0 }");
    
    ASSERT_METHOD_CALL("on_queue", call);
    ASSERT_GLOBAL_VARIABLE("queue", call->childAtIndex(0));
    
    closure = static_cast<ast::Closure*>(call->childAtIndex(1));
    ASSERT_CLOSURE(closure);
    
    op = static_cast<ast::binary_operator*>(closure->getBody()->childAtIndex(0));
    ASSERT_GLOBAL_ASSIGNMENT(op);
    ASSERT_GLOBAL_VARIABLE("queue", op->getLeftOperand());
}

TEST_F(ClosureParseTests, ClosedLocalVariableInMethod)
{
    ast::method_definition*     method;
    ast::binary_operator*       op;
    ast::MethodCall*           call;
    ast::Closure*               closure;
    std::vector<ast::Variable*> closedVariables;
    
    method = parse_method_def("method foo() {\n a = 42\n a.times() do { a = a + 1 }\n a.print()\n }");
    
    ASSERT_METHOD_DEFINITION("foo", method);
    
    // a = 42
    op = static_cast<ast::binary_operator*>(method->getBody()->childAtIndex(0));
    ASSERT_LOCAL_ASSIGNMENT(op);
    ASSERT_LOCAL_VARIABLE("a", op->getLeftOperand());
    
    // a.times()....
    op = static_cast<ast::binary_operator*>(method->getBody()->childAtIndex(1));
    ASSERT_OPERATOR(".", op);
    
    call = static_cast<ast::MethodCall*>(op->getRightOperand());
    ASSERT_METHOD_CALL("times", call);
    
    closure = static_cast<ast::Closure*>(call->childAtIndex(0));
    ASSERT_CLOSURE(closure);
    
    closedVariables = closure->getClosedVariables();
    ASSERT_EQ(1, closedVariables.size());
    ASSERT_EQ("a", closedVariables[0]->getIdentifier());
    
    op = static_cast<ast::binary_operator*>(closure->getBody()->childAtIndex(0));
    ASSERT_CLOSED_ASSIGNMENT(op);
    ASSERT_CLOSED_LOCAL_VARIABLE("a", op->getLeftOperand());
    
    op = static_cast<ast::binary_operator*>(op->getRightOperand());
    ASSERT_OPERATOR("+", op);
    ASSERT_CLOSED_LOCAL_VARIABLE("a", op->getLeftOperand());
    
    // a.print()
    op = static_cast<ast::binary_operator*>(method->getBody()->childAtIndex(2));
    ASSERT_OPERATOR(".", op);
    ASSERT_SHARED_LOCAL_VARIABLE("a", op->getLeftOperand());
}

TEST_F(ClosureParseTests, NestedClosedLocalVariablesInMethod)
{
    ast::method_definition*     method;
    //ast::binary_operator*       op;
    //ast::MethodCall*           call;
    ast::Closure*               closure;
    std::vector<ast::Variable*> closedVariables;
    
    method = parse_method_def("method foo() {\n  a = 42\n  b = 0\n do { a = a + 1 \n do { b = b + 5 }\n }\n a.print()\n b.print()\n }");
    
    closure = static_cast<ast::Closure*>(method->getBody()->childAtIndex(2));
    ASSERT_CLOSURE(closure);
    
    closedVariables = closure->getClosedVariables();
    ASSERT_EQ(2, closedVariables.size());
    ASSERT_EQ("a", closedVariables[0]->getIdentifier());
    ASSERT_EQ("b", closedVariables[1]->getIdentifier());
}
