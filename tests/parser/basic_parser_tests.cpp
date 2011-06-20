#include "tests/parser/parser_tests_base.h"
#include "parser/parse_error.h"

class BasicParserTest : public ParserTestsBase
{
};


TEST_F(BasicParserTest, ErrorTest)
{
    chime::parse_error* error;
    
    error = new chime::parse_error("yo %d", 1);
    
    ASSERT_EQ("yo 1", error->message());
    
    delete error;
}

TEST_F(BasicParserTest, ImportIdentifier)
{
    ast::node* node;
    
    node = parse("import Yo.Dog")->child_at_index(0);
    
    assert_import(node);
    assert_type("Yo.Dog", ((ast::import*)node)->importand());
}

TEST_F(BasicParserTest, MultipleImports)
{
    ast::node*   node;
    ast::import* import;
    
    node = parse("import Yo\nimport Sup\n");
    
    import = (ast::import*)node->child_at_index(0);
    assert_import(import);
    assert_type("Yo", import->importand());
    
    import = (ast::import*)node->child_at_index(1);
    assert_import(import);
    assert_type("Sup", import->importand());
}

TEST_F(BasicParserTest, SimpleImplementation)
{
    ast::Implementation* node;
    
    node = parse_implementation("implementation SomeClass {}");
    
    ASSERT_IMPLEMENTATION("SomeClass", NULL, node);
}

TEST_F(BasicParserTest, ImplementationWithSuperClass)
{
    ast::Implementation* node;
    
    node = parse_implementation("implementation SomeClass : SuperClass {}");
    
    ASSERT_IMPLEMENTATION("SomeClass", "SuperClass", node);
}

TEST_F(BasicParserTest, InstanceVariable)
{
    ast::Implementation*       node;
    ast::variable_definition* ivar;
    
    node = parse_implementation("implementation SomeClass { Foo bar }");
    
    ASSERT_IMPLEMENTATION("SomeClass", NULL, node);
    
    ivar = (ast::variable_definition*)node->getBody()->child_at_index(0);
    assert_variable_definition("Foo", "bar", ivar);
}

TEST_F(BasicParserTest, SimpleMethodDefinition)
{
    ast::Implementation*    node;
    ast::method_definition* method;
    
    node = parse_implementation("implementation SomeClass { method new() { } }");
    
    ASSERT_IMPLEMENTATION("SomeClass", NULL, node);
    
    method = (ast::method_definition*)node->getBody()->child_at_index(0);
    assert_method_definition("new", method);
}

TEST_F(BasicParserTest, MethodDefinitionWithOneParameter)
{
    ast::method_definition* method;
    
    method = parse_method_def("method new(arg1) { }");
    
    assert_method_definition("new", method);
    ASSERT_METHOD_PARAMETER(NULL, NULL, "arg1", method->getParameters()->childAtIndex(0));
}

TEST_F(BasicParserTest, MethodDefinitionWithOneTypedParameter)
{
    ast::method_definition* method;
    
    method = parse_method_def("method new(Hash arg1) { }");
    
    assert_method_definition("new", method);
    ASSERT_METHOD_PARAMETER("Hash", NULL, "arg1", method->getParameters()->childAtIndex(0));
}

TEST_F(BasicParserTest, MethodDefinitionWithOneTypedLabelledParameter)
{
    ast::method_definition* method;
    
    method = parse_method_def("method new(one:Hash arg1) { }");
    
    assert_method_definition("new", method);
    ASSERT_METHOD_PARAMETER("Hash", NULL, "arg1", method->getParameters()->childAtIndex(0));
}

TEST_F(BasicParserTest, MethodDefinitionWithTwoParameters)
{
    ast::method_definition* method;
    
    method = parse_method_def("method new(arg1, arg2) { }");
    
    assert_method_definition("new", method);
    ASSERT_METHOD_PARAMETER(NULL, NULL, "arg1", method->getParameters()->childAtIndex(0));
    ASSERT_METHOD_PARAMETER(NULL, NULL, "arg2", method->getParameters()->childAtIndex(1));
}

TEST_F(BasicParserTest, AssignmentExpression)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("a = b");
    op   = (ast::binary_operator*)node->child_at_index(0);
    
    ASSERT_OPERATOR("=", op);
    ASSERT_ENTITY("a", op->left_operand());
    ASSERT_ENTITY("b", op->right_operand());
}

TEST_F(BasicParserTest, ComplexOperatorExpression)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("a = b + c * d + e");
    op   = (ast::binary_operator*)node->child_at_index(0);
    
    ASSERT_OPERATOR("=", op);
    ASSERT_ENTITY("a", op->left_operand());
    
    op = (ast::binary_operator*)op->right_operand();
    ASSERT_OPERATOR("+", op);
    ASSERT_ENTITY("e", op->right_operand());
    
    op = (ast::binary_operator*)op->left_operand();
    ASSERT_OPERATOR("+", op);
    ASSERT_ENTITY("b", op->left_operand());
    ASSERT_OPERATOR("*", op->right_operand());
}

TEST_F(BasicParserTest, ExpressionWithParenthesis)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("a = (b + c) * d");
    op   = (ast::binary_operator*)node->child_at_index(0);
    
    ASSERT_OPERATOR("=", op);
    ASSERT_ENTITY("a", op->left_operand());
    
    op = (ast::binary_operator*)op->right_operand();
    ASSERT_OPERATOR("*", op);
    ASSERT_ENTITY("d", op->right_operand());
    
    op = (ast::binary_operator*)op->left_operand();
    ASSERT_OPERATOR("+", op);
    ASSERT_ENTITY("b", op->left_operand());
    ASSERT_ENTITY("c", op->right_operand());
}

TEST_F(BasicParserTest, AssignmentFromTypeMethodCall)
{
    ast::binary_operator* op;
    
    op = (ast::binary_operator*)parse("a = Foo.Bar.baz()")->child_at_index(0);
    
    assert_operator("=", op);
    assert_entity("a", op->left_operand());
    
    op = (ast::binary_operator*)op->right_operand();
    
    assert_operator(".", op);
    assert_type("Foo.Bar", op->left_operand());
    assert_method_call("baz", op->right_operand());
}

TEST_F(BasicParserTest, TopLevelMethod)
{
    ast::node* node;
    
    node = parse("method foo() {}");
    
    assert_method_definition("foo", node->child_at_index(0));
}

TEST_F(BasicParserTest, MethodParameter)
{
    ast::method_definition* method;
    
    method = parse_method_def("method foo(one:Bar baz) {}");
    
    assert_method_definition("foo", method);
    ASSERT_METHOD_PARAMETER("Bar", "one", "baz", method->getParameters()->childAtIndex(0));
}

TEST_F(BasicParserTest, ExpressionInMethodBody)
{
    ast::method_definition* method;
    ast::binary_operator*   op;
    
    method = parse_method_def("method foo() { a = b }");
    
    ASSERT_EQ(1, method->getBody()->child_count());
    
    op = (ast::binary_operator*)method->getBody()->child_at_index(0);
    
    assert_operator("=", op);
}

TEST_F(BasicParserTest, MethodCall)
{
    ast::node* node;
    
    node = parse("call()");
    
    assert_method_call("call", node->child_at_index(0));
}

TEST_F(BasicParserTest, TypeMethodCall)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("Type.call()");
    op   = (ast::binary_operator*)node->child_at_index(0);
    
    assert_operator(".", op);
    assert_type("Type", op->left_operand());
    assert_method_call("call", op->right_operand());
}

TEST_F(BasicParserTest, NamespacedTypeMethodCall)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("Some.Type.call()");
    op   = (ast::binary_operator*)node->child_at_index(0);
    
    assert_operator(".", op);
    assert_type("Some.Type", op->left_operand());
    assert_method_call("call", op->right_operand());
}

TEST_F(BasicParserTest, MethodCallArguments)
{
    ast::method_call* call;
    
    call = parse_method_call("call(a, b)");
    
    ASSERT_METHOD_CALL("call", call);
    ASSERT_ENTITY("a", call->child_at_index(0));
    ASSERT_ENTITY("b", call->child_at_index(1));
}

TEST_F(BasicParserTest, MethodCallArgumentWithParentheses)
{
    ast::method_call*     call;
    ast::binary_operator* op;
    
    call = parse_method_call("call((a + b) * c)");
    
    ASSERT_METHOD_CALL("call", call);
    
    op = (ast::binary_operator*)call->child_at_index(0);
    ASSERT_OPERATOR("*", op);
    ASSERT_ENTITY("c", op->right_operand());
    
    op = (ast::binary_operator*)op->left_operand();
    ASSERT_OPERATOR("+", op);
    ASSERT_ENTITY("a", op->left_operand());
    ASSERT_ENTITY("b", op->right_operand());
}

TEST_F(BasicParserTest, FunctionType)
{
    ast::variable_definition* node;
    
    node = (ast::variable_definition*)parse("Function() a")->child_at_index(0);
    
    assert_variable_definition("Function", "a", node);
}

TEST_F(BasicParserTest, MethodCallWithBlock)
{
    ast::method_call* call;
    
    call = (ast::method_call*)parse("call(a, b) do {}")->child_at_index(0);
    
    ASSERT_METHOD_CALL("call", call);
    ASSERT_ENTITY("a", call->child_at_index(0));
    ASSERT_ENTITY("b", call->child_at_index(1));
    ASSERT_BLOCK(call->child_at_index(2));
}

TEST_F(BasicParserTest, MethodCallWithBlockAndBody)
{
    ast::method_call* call;
    
    call = (ast::method_call*)parse("call() do { a = \"string\" }")->child_at_index(0);
    
    ASSERT_METHOD_CALL("call", call);
    ASSERT_BLOCK(call->child_at_index(0));
    ASSERT_OPERATOR("=", call->child_at_index(0)->child_at_index(0));
}

TEST_F(BasicParserTest, MethodCallWithBlockParameters)
{
    ast::method_call* call;
    
    call = parse_method_call("call() do (a) { }");
    
    ASSERT_METHOD_CALL("call", call);
    ASSERT_BLOCK(call->child_at_index(0));
}

TEST_F(BasicParserTest, MethodCallWithLablledParams)
{
    ast::method_call* call;
    
    call = parse_method_call("foo(bar:a, baz:b)");
    
    ASSERT_METHOD_CALL("foo", call);
    ASSERT_ENTITY("a", call->child_at_index(0));
    ASSERT_ENTITY("b", call->child_at_index(1));
}

TEST_F(BasicParserTest, MethodCallWithLabelledBlock)
{
    ast::method_call* call;
    
    call = parse_method_call("on_queue(queue, async:) do { queue = 0 }");
    
    ASSERT_METHOD_CALL("on_queue", call);
    ASSERT_ENTITY("queue", call->child_at_index(0));
    ASSERT_BLOCK(call->child_at_index(1));
}

TEST_F(BasicParserTest, MethodWithOperatorsOnType)
{
    ast::method_definition* method;
    ast::binary_operator*   op;
    
    method = parse_method_def("method foo() { Bar.baz() }");
    
    assert_method_definition("foo", method);
    
    op = (ast::binary_operator*)method->getBody()->child_at_index(0);
    assert_operator(".", op);
    assert_type("Bar", op->left_operand());
    assert_method_call("baz", op->right_operand());
}
