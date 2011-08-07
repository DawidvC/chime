#include "parser_tests_base.h"
#include "compiler/parser/parse_error.h"

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
    
    ASSERT_IMPORT(node);
    ASSERT_TYPE("Yo.Dog", ((ast::Import*)node)->getImportand());
}

TEST_F(BasicParserTest, MultipleImports)
{
    ast::node*   node;
    ast::Import* import;
    
    node = parse("import Yo\nimport Sup\n");
    
    import = (ast::Import*)node->child_at_index(0);
    ASSERT_IMPORT(import);
    ASSERT_TYPE("Yo", import->getImportand());
    
    import = (ast::Import*)node->child_at_index(1);
    ASSERT_IMPORT(import);
    ASSERT_TYPE("Sup", import->getImportand());
}

TEST_F(BasicParserTest, ImportString)
{
    ast::Import* import;
    
    import = (ast::Import*)parse("import \"a/file\"\n")->child_at_index(0);
    
    ASSERT_IMPORT(import);
    ASSERT_LITERAL_STRING("a/file", import->getImportand());
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
    ASSERT_VARIABLE_DEFINITION("Foo", "bar", ivar);
}

TEST_F(BasicParserTest, SimpleMethodDefinition)
{
    ast::Implementation*    node;
    ast::method_definition* method;
    
    node = parse_implementation("implementation SomeClass { method new() { } }");
    
    ASSERT_IMPLEMENTATION("SomeClass", NULL, node);
    
    method = (ast::method_definition*)node->getBody()->child_at_index(0);
    ASSERT_METHOD_DEFINITION("new", method);
}

TEST_F(BasicParserTest, MethodDefinitionWithOneParameter)
{
    ast::method_definition* method;
    
    method = parse_method_def("method new(arg1) { }");
    
    ASSERT_METHOD_DEFINITION("new", method);
    ASSERT_METHOD_PARAMETER(NULL, NULL, "arg1", method->getParameters()->childAtIndex(0));
}

TEST_F(BasicParserTest, MethodDefinitionWithOneTypedParameter)
{
    ast::method_definition* method;
    
    method = parse_method_def("method new(Hash arg1) { }");
    
    ASSERT_METHOD_DEFINITION("new", method);
    ASSERT_METHOD_PARAMETER("Hash", NULL, "arg1", method->getParameters()->childAtIndex(0));
}

TEST_F(BasicParserTest, MethodDefinitionWithOneTypedLabelledParameter)
{
    ast::method_definition* method;
    
    method = parse_method_def("method new(one:Hash arg1) { }");
    
    ASSERT_METHOD_DEFINITION("new", method);
    ASSERT_METHOD_PARAMETER("Hash", NULL, "arg1", method->getParameters()->childAtIndex(0));
}

TEST_F(BasicParserTest, MethodDefinitionWithTwoParameters)
{
    ast::method_definition* method;
    
    method = parse_method_def("method new(arg1, arg2) { }");
    
    ASSERT_METHOD_DEFINITION("new", method);
    ASSERT_METHOD_PARAMETER(NULL, NULL, "arg1", method->getParameters()->childAtIndex(0));
    ASSERT_METHOD_PARAMETER(NULL, NULL, "arg2", method->getParameters()->childAtIndex(1));
}

TEST_F(BasicParserTest, MethodDefinitionWithFunction)
{
    ast::method_definition* method;
    
    method = parse_method_def("method foo(a, label:Function(data) block) {}");
    
    ASSERT_METHOD_DEFINITION("foo", method);
    ASSERT_METHOD_PARAMETER(NULL,       NULL, "a",     method->getParameters()->childAtIndex(0));
    ASSERT_METHOD_PARAMETER("Function", NULL, "block", method->getParameters()->childAtIndex(1));
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
    
    ASSERT_OPERATOR("=", op);
    ASSERT_ENTITY("a", op->left_operand());
    
    op = (ast::binary_operator*)op->right_operand();
    
    ASSERT_OPERATOR(".", op);
    ASSERT_TYPE("Foo.Bar", op->left_operand());
    ASSERT_METHOD_CALL("baz", op->right_operand());
}

TEST_F(BasicParserTest, TopLevelMethod)
{
    ast::node* node;
    
    node = parse("method foo() {}");
    
    ASSERT_METHOD_DEFINITION("foo", node->child_at_index(0));
}

TEST_F(BasicParserTest, MethodParameter)
{
    ast::method_definition* method;
    
    method = parse_method_def("method foo(one:Bar baz) {}");
    
    ASSERT_METHOD_DEFINITION("foo", method);
    ASSERT_METHOD_PARAMETER("Bar", "one", "baz", method->getParameters()->childAtIndex(0));
}

TEST_F(BasicParserTest, ExpressionInMethodBody)
{
    ast::method_definition* method;
    ast::binary_operator*   op;
    
    method = parse_method_def("method foo() { a = b }");
    
    ASSERT_EQ(1, method->getBody()->child_count());
    
    op = (ast::binary_operator*)method->getBody()->child_at_index(0);
    
    ASSERT_OPERATOR("=", op);
}

TEST_F(BasicParserTest, MethodCall)
{
    ast::node* node;
    
    node = parse("call()");
    
    ASSERT_METHOD_CALL("call", node->child_at_index(0));
}

TEST_F(BasicParserTest, TypeMethodCall)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("Type.call()");
    op   = (ast::binary_operator*)node->child_at_index(0);
    
    ASSERT_OPERATOR(".", op);
    ASSERT_TYPE("Type", op->left_operand());
    ASSERT_METHOD_CALL("call", op->right_operand());
}

TEST_F(BasicParserTest, NamespacedTypeMethodCall)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("Some.Type.call()");
    op   = (ast::binary_operator*)node->child_at_index(0);
    
    ASSERT_OPERATOR(".", op);
    ASSERT_TYPE("Some.Type", op->left_operand());
    ASSERT_METHOD_CALL("call", op->right_operand());
}

TEST_F(BasicParserTest, GetProperty)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("b = a.prop");
    op   = (ast::binary_operator*)node->child_at_index(0);
    
    ASSERT_OPERATOR("=", op);
    ASSERT_ENTITY("b", op->left_operand());
    
    op   = (ast::binary_operator*)op->right_operand();
    ASSERT_ENTITY("a", op->left_operand());
    ASSERT_METHOD_CALL("prop", op->right_operand());
}

TEST_F(BasicParserTest, SetProperty)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("a.prop = b");
    op   = (ast::binary_operator*)node->child_at_index(0);
    
    ASSERT_OPERATOR(".", op);
    ASSERT_ENTITY("a", op->left_operand());
    ASSERT_METHOD_CALL("prop=", op->right_operand());
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
    
    ASSERT_VARIABLE_DEFINITION("Function", "a", node);
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
    
    ASSERT_METHOD_DEFINITION("foo", method);
    
    op = (ast::binary_operator*)method->getBody()->child_at_index(0);
    ASSERT_OPERATOR(".", op);
    ASSERT_TYPE("Bar", op->left_operand());
    ASSERT_METHOD_CALL("baz", op->right_operand());
}

TEST_F(BasicParserTest, PropertyWithGetAndSet)
{
    ast::PropertyDefinition* property;
    
    property = (ast::PropertyDefinition*)parse("property foo(value) { get { return a } set { a = value} }")->child_at_index(0);
    
    ASSERT_PROPERTY_DEFINITION("foo", property);
    ASSERT_METHOD_PARAMETER(NULL, NULL, "value", property->getParameters()->childAtIndex(0));
}
