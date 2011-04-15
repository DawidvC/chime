#include "parser_tests_base.h"

class BasicParserTest : public ParserTestsBase
{
};

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
    ast::implementation* node;
    
    node = parse_implementation("implementation SomeClass {}");
    
    assert_implementation("SomeClass", NULL, node);
}

TEST_F(BasicParserTest, ImplementationWithSuperClass)
{
    ast::implementation* node;
    
    node = parse_implementation("implementation SomeClass : SuperClass {}");
    
    assert_implementation("SomeClass", "SuperClass", node);
}

TEST_F(BasicParserTest, InstanceVariable)
{
    ast::implementation*       node;
    ast::variable_definition* ivar;
    
    node = parse_implementation("implementation SomeClass { Foo bar }");
    
    assert_implementation("SomeClass", NULL, node);
    
    ivar = (ast::variable_definition*)node->child_at_index(0);
    assert_variable_definition("Foo", "bar", ivar);
}

TEST_F(BasicParserTest, SimpleMethodDefinition)
{
    ast::implementation*    node;
    ast::method_definition* method;
    
    node = parse_implementation("implementation SomeClass { method new() { } }");
    
    assert_implementation("SomeClass", NULL, node);
    
    method = (ast::method_definition*)node->child_at_index(0);
    assert_method_definition("new", method);
}

TEST_F(BasicParserTest, MethodDefinitionWithOneParameter)
{
    ast::method_definition* method;
    
    method = parse_method_def("method new(arg1) { }");
    
    assert_method_definition("new", method);
    assert_method_parameter(NULL, NULL, "arg1", method->parameter_at_index(0));
}

TEST_F(BasicParserTest, MethodDefinitionWithOneTypedParameter)
{
    ast::method_definition* method;
    
    method = parse_method_def("method new(Hash arg1) { }");
    
    assert_method_definition("new", method);
    assert_method_parameter("Hash", NULL, "arg1", method->parameter_at_index(0));
}

TEST_F(BasicParserTest, MethodDefinitionWithOneTypedLabelledParameter)
{
    ast::method_definition* method;
    
    method = parse_method_def("method new(one:Hash arg1) { }");
    
    assert_method_definition("new", method);
    assert_method_parameter("Hash", NULL, "arg1", method->parameter_at_index(0));
}

TEST_F(BasicParserTest, MethodDefinitionWithTwoParameters)
{
    ast::method_definition* method;
    
    method = parse_method_def("method new(arg1, arg2) { }");
    
    assert_method_definition("new", method);
    assert_method_parameter(NULL, NULL, "arg1", method->parameter_at_index(0));
    assert_method_parameter(NULL, NULL, "arg2", method->parameter_at_index(1));
}

TEST_F(BasicParserTest, AssignmentExpression)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("a = b");
    op   = (ast::binary_operator*)node->child_at_index(0);
    
    assert_operator("=", op);
    assert_entity("a", op->left_operand());
    assert_entity("b", op->right_operand());
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
    assert_method_parameter("Bar", "one", "baz", method->parameter_at_index(0));
}

TEST_F(BasicParserTest, ExpressionInMethodBody)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("method foo() { a = b }");
    node = node->child_at_index(0); // get the method def
    
    ASSERT_EQ(1, (int)node->child_count());
    
    op = (ast::binary_operator*)node->child_at_index(0);
    
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
    
    call = (ast::method_call*)parse("call(a, b)")->child_at_index(0);
    
    assert_method_call("call", call);
    assert_entity("a", call->child_at_index(0));
    assert_entity("b", call->child_at_index(1));
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
    
    assert_method_call("call", call);
    assert_entity("a", call->child_at_index(0));
    assert_entity("b", call->child_at_index(1));
    assert_block(call->child_at_index(2));
}

TEST_F(BasicParserTest, MethodCallWithBlockAndBody)
{
    ast::method_call* call;
    
    call = (ast::method_call*)parse("call() do { a = \"string\" }")->child_at_index(0);
    
    assert_method_call("call", call);
    assert_block(call->child_at_index(0));
    assert_operator("=", call->child_at_index(0)->child_at_index(0));
}

TEST_F(BasicParserTest, MethodCallWithBlockParameters)
{
    ast::method_call* call;
    
    call = (ast::method_call*)parse("call() do (a) { }")->child_at_index(0);
    
    assert_method_call("call", call);
    assert_block(call->child_at_index(0));
}

TEST_F(BasicParserTest, MethodWithOperatorsOnType)
{
    ast::method_definition* method;
    ast::binary_operator*   op;
    
    method = parse_method_def("method foo() { Bar.baz() }");
    
    assert_method_definition("foo", method);
    
    op = (ast::binary_operator*)method->child_at_index(0);
    assert_operator(".", op);
    assert_type("Bar", op->left_operand());
    assert_method_call("baz", op->right_operand());
}
