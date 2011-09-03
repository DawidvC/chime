#include "parser_tests_base.h"

class ClosureParseTests : public ParserTestsBase
{
};

TEST_F(ClosureParseTests, MethodCallWithClosure)
{
    ast::method_call* call;
    
    call = parse_method_call("call(a, b) do {}");
    
    ASSERT_METHOD_CALL("call", call);
    ASSERT_GLOBAL_VARIABLE("a", call->childAtIndex(0));
    ASSERT_GLOBAL_VARIABLE("b", call->childAtIndex(1));
    ASSERT_CLOSURE(call->childAtIndex(2));
}

TEST_F(ClosureParseTests, MethodCallWithClosureParameters)
{
    ast::method_call* call;
    
    call = parse_method_call("call() do (a) { }");
    
    ASSERT_METHOD_CALL("call", call);
    ASSERT_CLOSURE(call->child_at_index(0));
}

TEST_F(ClosureParseTests, MethodCallWithClosureAndBody)
{
    ast::method_call*     call;
    ast::Closure*         closure;
    ast::binary_operator* op;
    
    call = parse_method_call("call() do { a = \"string\" }");
    
    ASSERT_METHOD_CALL("call", call);
    
    closure = static_cast<ast::Closure*>(call->childAtIndex(0));
    ASSERT_CLOSURE(closure);
    
    op = static_cast<ast::binary_operator*>(closure->getBody()->childAtIndex(0));
    ASSERT_OPERATOR("=", op);
    ASSERT_LOCAL_VARIABLE("a", op->getLeftOperand());
}

TEST_F(ClosureParseTests, MethodCallWithLabelledClosure)
{
    ast::method_call*     call;
    ast::Closure*         closure;
    ast::binary_operator* op;
    
    call = parse_method_call("on_queue(queue, async:) do { queue = 0 }");
    
    ASSERT_METHOD_CALL("on_queue", call);
    ASSERT_GLOBAL_VARIABLE("queue", call->childAtIndex(0));
    
    closure = static_cast<ast::Closure*>(call->childAtIndex(1));
    ASSERT_CLOSURE(closure);
    
    op = static_cast<ast::binary_operator*>(closure->getBody()->childAtIndex(0));
    ASSERT_OPERATOR("=", op);
    ASSERT_CLOSED_GLOBAL_VARIABLE("queue", op->getLeftOperand());
}

TEST_F(ClosureParseTests, ClosedLocalVariableInMethod)
{
    ast::method_definition*     method;
    ast::binary_operator*       op;
    ast::method_call*           call;
    ast::Closure*               closure;
    std::vector<ast::Variable*> closedVariables;
    
    method = parse_method_def("method foo() {\n a = 42\n a.times() do { a.print() }\n }");
    
    ASSERT_METHOD_DEFINITION("foo", method);
    
    op = static_cast<ast::binary_operator*>(method->getBody()->childAtIndex(0));
    ASSERT_OPERATOR("=", op);
    ASSERT_LOCAL_VARIABLE("a", op->getLeftOperand());
    
    op = static_cast<ast::binary_operator*>(method->getBody()->childAtIndex(1));
    ASSERT_OPERATOR(".", op);
    
    call = static_cast<ast::method_call*>(op->getRightOperand());
    ASSERT_METHOD_CALL("times", call);
    
    closure = static_cast<ast::Closure*>(call->childAtIndex(0));
    ASSERT_CLOSURE(closure);
    
    closedVariables = closure->getClosedVariables();
    ASSERT_EQ(1, closedVariables.size());
    ASSERT_EQ("a", closedVariables[0]->getIdentifier());
    
    op = static_cast<ast::binary_operator*>(closure->getBody()->childAtIndex(0));
    ASSERT_OPERATOR(".", op);
    ASSERT_CLOSED_LOCAL_VARIABLE("a", op->getLeftOperand());
    ASSERT_METHOD_CALL("print", op->getRightOperand());
}
