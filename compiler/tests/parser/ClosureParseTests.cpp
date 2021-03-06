#include "parser_tests_base.h"

class ClosureParseTests : public ParserTestsBase
{
};

TEST_F(ClosureParseTests, MethodCallWithClosure)
{
    chime::MethodCall* call;
    
    call = parseMethodCall("call(a, b) do {}");
    
    ASSERT_METHOD_CALL("call:::", call);
    ASSERT_GLOBAL_VARIABLE("a", call->childAtIndex(0));
    ASSERT_GLOBAL_VARIABLE("b", call->childAtIndex(1));
    ASSERT_CLOSURE(call->childAtIndex(2));
}

TEST_F(ClosureParseTests, MethodCallWithClosureParameters)
{
    chime::MethodCall* call;
    chime::Closure*     closure;
    
    call = parseMethodCall("call() do (a) { }");
    
    ASSERT_METHOD_CALL("call:", call);
    
    closure = dynamic_cast<chime::Closure*>(call->childAtIndex(0));
    ASSERT_CLOSURE(closure);
    
    ASSERT_METHOD_PARAMETER(NULL, NULL, "a", closure->getParameters()[0]);
}

TEST_F(ClosureParseTests, MethodCallWithClosureAndBody)
{
    chime::MethodCall*     call;
    chime::Closure*         closure;
    chime::binary_operator* op;
    
    call = parseMethodCall("call() do { a = \"string\" }");
    
    ASSERT_METHOD_CALL("call:", call);
    
    closure = dynamic_cast<chime::Closure*>(call->childAtIndex(0));
    ASSERT_CLOSURE(closure);
    
    op = dynamic_cast<chime::binary_operator*>(closure->getBody()->childAtIndex(0));
    ASSERT_LOCAL_ASSIGNMENT(op);
    ASSERT_LOCAL_VARIABLE("a", op->getLeftOperand());
}

TEST_F(ClosureParseTests, AnonymousFunctionDefinition)
{
    chime::binary_operator* op;
    
    op = parseOperator("f = do { return 5; }");
    
    ASSERT_GLOBAL_ASSIGNMENT(op);
    ASSERT_GLOBAL_VARIABLE("f", op->getLeftOperand());
    ASSERT_CLOSURE(op->getRightOperand());
}

TEST_F(ClosureParseTests, MethodCallWithLabelledClosure)
{
    chime::MethodCall*      call;
    chime::Closure*         closure;
    chime::binary_operator* op;
    
    call = parseMethodCall("on_queue(queue, async:) do { queue = 0 }");
    
    ASSERT_METHOD_CALL("on_queue:async:", call);
    ASSERT_GLOBAL_VARIABLE("queue", call->childAtIndex(0));
    
    closure = dynamic_cast<chime::Closure*>(call->childAtIndex(1));
    ASSERT_CLOSURE(closure);
    
    op = dynamic_cast<chime::binary_operator*>(closure->getBody()->childAtIndex(0));
    ASSERT_CLOSED_ASSIGNMENT(op);
    ASSERT_CLOSED_GLOBAL_VARIABLE("queue", op->getLeftOperand());
}

TEST_F(ClosureParseTests, ClosedLocalVariableInMethod)
{
    chime::MethodDefinition*      method;
    chime::binary_operator*       op;
    chime::MethodCall*            call;
    chime::Closure*               closure;
    std::vector<chime::Variable*> closedVariables;
    
    method = parseFirst<chime::MethodDefinition*>("method foo() {\n a = 42\n a.times() do { a = a + 1 }\n a.print()\n }");
    
    ASSERT_METHOD_DEFINITION("foo", method);
    
    // a = 42
    op = dynamic_cast<chime::binary_operator*>(method->getBody()->childAtIndex(0));
    ASSERT_LOCAL_ASSIGNMENT(op);
    ASSERT_LOCAL_VARIABLE("a", op->getLeftOperand());
    
    // a.times()....
    op = dynamic_cast<chime::binary_operator*>(method->getBody()->childAtIndex(1));
    ASSERT_OPERATOR(".", op);
    
    call = dynamic_cast<chime::MethodCall*>(op->getRightOperand());
    ASSERT_METHOD_CALL("times:", call);
    
    closure = dynamic_cast<chime::Closure*>(call->childAtIndex(0));
    ASSERT_CLOSURE(closure);
    ASSERT_EQ("Root.foo_closure_1", closure->getIdentifier());
    
    closedVariables = closure->getClosedVariables();
    ASSERT_EQ(1, closedVariables.size());
    ASSERT_EQ("a", closedVariables[0]->getIdentifier());
    
    op = dynamic_cast<chime::binary_operator*>(closure->getBody()->childAtIndex(0));
    ASSERT_CLOSED_ASSIGNMENT(op);
    ASSERT_CLOSED_LOCAL_VARIABLE("a", op->getLeftOperand());
    
    op = dynamic_cast<chime::binary_operator*>(op->getRightOperand());
    ASSERT_OPERATOR("+", op);
    ASSERT_CLOSED_LOCAL_VARIABLE("a", op->getLeftOperand());
    
    // a.print()
    op = dynamic_cast<chime::binary_operator*>(method->getBody()->childAtIndex(2));
    ASSERT_OPERATOR(".", op);
    ASSERT_SHARED_LOCAL_VARIABLE("a", op->getLeftOperand());
}

TEST_F(ClosureParseTests, NestedClosedLocalVariablesInMethod)
{
    chime::MethodDefinition*      method;
    //chime::binary_operator*       op;
    //chime::MethodCall*            call;
    chime::Closure*               closure;
    std::vector<chime::Variable*> closedVariables;
    
    method = parseFirst<chime::MethodDefinition*>("method foo() {\n  a = 42\n  b = 0\n do { a = a + 1 \n do { b = b + 5 }\n }\n a.print()\n b.print()\n }");
    
    closure = dynamic_cast<chime::Closure*>(method->getBody()->childAtIndex(2));
    ASSERT_CLOSURE(closure);
    
    closedVariables = closure->getClosedVariables();
    ASSERT_EQ(2, closedVariables.size());
    ASSERT_EQ("a", closedVariables[0]->getIdentifier());
    ASSERT_EQ("b", closedVariables[1]->getIdentifier());
}

TEST_F(ClosureParseTests, MethodParameterUsedInClosure)
{
    chime::MethodDefinition* method;
    chime::Closure*          closure;
    chime::BinaryOperator*   op;
    
    method = parseFirst<chime::MethodDefinition*>("method foo(a) { do { a += 1 } }");
    
    ASSERT_METHOD_DEFINITION("foo:", method);
    
    closure = dynamic_cast<chime::Closure*>(method->getBody()->childAtIndex(0));
    ASSERT_CLOSURE(closure);
    
    op = dynamic_cast<chime::BinaryOperator*>(closure->getBody()->childAtIndex(0));
    ASSERT_CLOSED_LOCAL_VARIABLE("a", op->getLeftOperand());
}

TEST_F(ClosureParseTests, SelfInClosure)
{
    chime::MethodDefinition* method;
    chime::Closure*          closure;
    chime::BinaryOperator*   op;
    
    method = parseFirst<chime::MethodDefinition*>("method foo(a) { do { self.bar() } }");
    
    ASSERT_METHOD_DEFINITION("foo:", method);
    
    closure = dynamic_cast<chime::Closure*>(method->getBody()->childAtIndex(0));
    ASSERT_CLOSURE(closure);
    
    op = dynamic_cast<chime::BinaryOperator*>(closure->getBody()->childAtIndex(0));
    ASSERT_CLOSED_LITERAL_SELF(op->getLeftOperand());
}

TEST_F(ClosureParseTests, ClosureIdentifierInImplementation)
{
    chime::Implementation*   implementation;
    chime::MethodDefinition* method;
    chime::Closure*          closure;
    
    implementation = parseFirst<chime::Implementation*>("implementation Foo { method bar(a, b) { do { return a }\n do { return b } } }");
    method         = dynamic_cast<chime::MethodDefinition*>(implementation->getBody()->childAtIndex(0));
    
    closure = dynamic_cast<chime::Closure*>(method->getBody()->childAtIndex(0));
    ASSERT_CLOSURE(closure);
    ASSERT_EQ("Foo.bar::_closure_1", closure->getIdentifier());
    
    closure = dynamic_cast<chime::Closure*>(method->getBody()->childAtIndex(1));
    ASSERT_CLOSURE(closure);
    ASSERT_EQ("Foo.bar::_closure_2", closure->getIdentifier());
}

TEST_F(ClosureParseTests, ClosedGlobalVariable)
{
    chime::Node*           node;
    chime::BinaryOperator* op;
    chime::Closure*        closure;
    
    node = parse("a = 1\ndo { a = 2 }\n a.print()");
    
    op = dynamic_cast<chime::BinaryOperator*>(node->childAtIndex(0));
    ASSERT_GLOBAL_VARIABLE("a", op->getLeftOperand());
    
    closure = dynamic_cast<chime::Closure*>(node->childAtIndex(1));
    ASSERT_CLOSURE(closure);
    
    op = dynamic_cast<chime::BinaryOperator*>(closure->getBody()->childAtIndex(0));
    ASSERT_CLOSED_GLOBAL_VARIABLE("a", op->getLeftOperand());
}