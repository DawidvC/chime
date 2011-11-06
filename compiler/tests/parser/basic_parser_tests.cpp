#include "parser_tests_base.h"
#include "compiler/parser/ParseError.h"

class BasicParserTest : public ParserTestsBase
{
};


TEST_F(BasicParserTest, ErrorTest)
{
    chime::ParseErrorRef error;
    
    error = chime::ParseErrorRef(new chime::ParseError("yo %d", 1));
    
    ASSERT_EQ("yo 1", error->message());
}

TEST_F(BasicParserTest, ImportIdentifier)
{
    ast::Node* node;
    
    node = parse("import Yo.Dog")->childAtIndex(0);
    
    ASSERT_IMPORT(node);
    ASSERT_TYPE("Yo.Dog", static_cast<ast::Import*>(node)->getImportand());
}

TEST_F(BasicParserTest, MultipleImports)
{
    ast::node*   node;
    ast::Import* import;
    
    node = parse("import Yo\nimport Sup\n");
    
    import = static_cast<ast::Import*>(node->childAtIndex(0));
    ASSERT_IMPORT(import);
    ASSERT_TYPE("Yo", import->getImportand());
    
    import = static_cast<ast::Import*>(node->childAtIndex(1));
    ASSERT_IMPORT(import);
    ASSERT_TYPE("Sup", import->getImportand());
}

TEST_F(BasicParserTest, ImportString)
{
    ast::Import* import;
    
    import = static_cast<ast::Import*>(parse("import \"a/file\"\n")->childAtIndex(0));
    
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
    ast::Implementation* node;
    ast::Attribute*      ivar;
    
    node = parse_implementation("implementation SomeClass { attribute bar }");
    
    ASSERT_IMPLEMENTATION("SomeClass", NULL, node);
    
    ivar = (ast::Attribute*)node->getBody()->childAtIndex(0);
    ASSERT_ATTRIBUTE("bar", ivar);
}

TEST_F(BasicParserTest, AccessInstanceVariable)
{
    ast::Implementation*    node;
    ast::method_definition* method;
    ast::binary_operator*   op;
    
    node = parse_implementation("implementation SomeClass { attribute bar\n method foo() { bar = 0 } }");
    
    ASSERT_IMPLEMENTATION("SomeClass", NULL, node);
    
    method = static_cast<ast::method_definition*>(node->getBody()->childAtIndex(1));
    ASSERT_METHOD_DEFINITION("foo", method);
    
    op = static_cast<ast::binary_operator*>(method->getBody()->childAtIndex(0));
    ASSERT_INSTANCE_ASSIGNMENT(op);
    ASSERT_INSTANCE_VARIABLE("bar", op->getLeftOperand());
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

TEST_F(BasicParserTest, AssignmentExpression)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("a = b");
    op   = (ast::binary_operator*)node->childAtIndex(0);
    
    ASSERT_GLOBAL_ASSIGNMENT(op);
    ASSERT_GLOBAL_VARIABLE("a", op->getLeftOperand());
    ASSERT_GLOBAL_VARIABLE("b", op->getRightOperand());
}

TEST_F(BasicParserTest, ComplexOperatorExpression)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("a = b + c * d + e");
    op   = static_cast<ast::binary_operator*>(node->childAtIndex(0));
    
    ASSERT_GLOBAL_ASSIGNMENT(op);
    ASSERT_GLOBAL_VARIABLE("a", op->getLeftOperand());
    
    op = static_cast<ast::binary_operator*>(op->getRightOperand());
    ASSERT_OPERATOR("+", op);
    ASSERT_GLOBAL_VARIABLE("e", op->getRightOperand());
    
    op = static_cast<ast::binary_operator*>(op->getLeftOperand());
    ASSERT_OPERATOR("+", op);
    ASSERT_GLOBAL_VARIABLE("b", op->getLeftOperand());
    ASSERT_OPERATOR("*", op->getRightOperand());
}

TEST_F(BasicParserTest, ExpressionWithParenthesis)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("a = (b + c) * d");
    op   = (ast::binary_operator*)node->childAtIndex(0);
    
    ASSERT_GLOBAL_ASSIGNMENT(op);
    ASSERT_GLOBAL_VARIABLE("a", op->getLeftOperand());
    
    op = (ast::binary_operator*)op->getRightOperand();
    ASSERT_OPERATOR("*", op);
    ASSERT_GLOBAL_VARIABLE("d", op->getRightOperand());
    
    op = (ast::binary_operator*)op->getLeftOperand();
    ASSERT_OPERATOR("+", op);
    ASSERT_GLOBAL_VARIABLE("b", op->getLeftOperand());
    ASSERT_GLOBAL_VARIABLE("c", op->getRightOperand());
}

TEST_F(BasicParserTest, AssignmentFromTypeMethodCall)
{
    ast::binary_operator* op;
    
    op = (ast::binary_operator*)parse("a = Foo.Bar.baz()")->childAtIndex(0);
    
    ASSERT_GLOBAL_ASSIGNMENT(op);
    ASSERT_GLOBAL_VARIABLE("a", op->getLeftOperand());
    
    op = (ast::binary_operator*)op->getRightOperand();
    
    ASSERT_OPERATOR(".", op);
    ASSERT_TYPE("Foo.Bar", op->getLeftOperand());
    ASSERT_METHOD_CALL("baz", op->getRightOperand());
}

TEST_F(BasicParserTest, ExpressionInMethodBody)
{
    ast::method_definition* method;
    ast::binary_operator*   op;
    
    method = parse_method_def("method foo() { a = b }");
    
    ASSERT_EQ(1, method->getBody()->childCount());
    
    op = static_cast<ast::binary_operator*>(method->getBody()->childAtIndex(0));
    
    ASSERT_LOCAL_ASSIGNMENT(op);
}

TEST_F(BasicParserTest, MethodCall)
{
    ast::node* node;
    
    node = parse("call()");
    
    ASSERT_METHOD_CALL("call", node->childAtIndex(0));
}

TEST_F(BasicParserTest, TypeMethodCall)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("Type.call()");
    op   = (ast::binary_operator*)node->childAtIndex(0);
    
    ASSERT_OPERATOR(".", op);
    ASSERT_TYPE("Type", op->left_operand());
    ASSERT_METHOD_CALL("call", op->right_operand());
}

TEST_F(BasicParserTest, NamespacedTypeMethodCall)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("Some.Type.call()");
    op   = (ast::binary_operator*)node->childAtIndex(0);
    
    ASSERT_OPERATOR(".", op);
    ASSERT_TYPE("Some.Type", op->left_operand());
    ASSERT_METHOD_CALL("call", op->right_operand());
}

TEST_F(BasicParserTest, GetProperty)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("b = a.prop");
    op   = (ast::binary_operator*)node->childAtIndex(0);
    
    ASSERT_GLOBAL_ASSIGNMENT(op);
    ASSERT_GLOBAL_VARIABLE("b", op->getLeftOperand());
    
    op   = (ast::binary_operator*)op->getRightOperand();
    ASSERT_GLOBAL_VARIABLE("a", op->getLeftOperand());
    ASSERT_METHOD_CALL("prop", op->getRightOperand());
}

TEST_F(BasicParserTest, SetProperty)
{
    ast::node*            node;
    ast::binary_operator* op;
    
    node = parse("a.prop = b");
    op   = (ast::binary_operator*)node->childAtIndex(0);
    
    ASSERT_OPERATOR(".", op);
    ASSERT_GLOBAL_VARIABLE("a", op->getLeftOperand());
    ASSERT_METHOD_CALL("prop=", op->getRightOperand());
}

TEST_F(BasicParserTest, MethodCallArguments)
{
    ast::MethodCall* call;
    
    call = parseMethodCall("call(a, b)");
    
    ASSERT_METHOD_CALL("call::", call);
    ASSERT_GLOBAL_VARIABLE("a", call->childAtIndex(0));
    ASSERT_GLOBAL_VARIABLE("b", call->childAtIndex(1));
}

TEST_F(BasicParserTest, MethodCallArgumentWithParentheses)
{
    ast::MethodCall*     call;
    ast::binary_operator* op;
    
    call = parseMethodCall("call((a + b) * c)");
    
    ASSERT_METHOD_CALL("call:", call);
    
    op = (ast::binary_operator*)call->childAtIndex(0);
    ASSERT_OPERATOR("*", op);
    ASSERT_GLOBAL_VARIABLE("c", op->getRightOperand());
    
    op = (ast::binary_operator*)op->getLeftOperand();
    ASSERT_OPERATOR("+", op);
    ASSERT_GLOBAL_VARIABLE("a", op->getLeftOperand());
    ASSERT_GLOBAL_VARIABLE("b", op->getRightOperand());
}

TEST_F(BasicParserTest, MethodCallWithLabelledParams)
{
    ast::MethodCall* call;
    
    call = parseMethodCall("foo(a, bar:b)");
    
    ASSERT_METHOD_CALL("foo:bar:", call);
    ASSERT_GLOBAL_VARIABLE("a", call->childAtIndex(0));
    ASSERT_GLOBAL_VARIABLE("b", call->childAtIndex(1));
}

TEST_F(BasicParserTest, MethodWithOperatorsOnType)
{
    ast::method_definition* method;
    ast::binary_operator*   op;
    
    method = parse_method_def("method foo() { Bar.baz() }");
    
    ASSERT_METHOD_DEFINITION("foo", method);
    
    op = static_cast<ast::binary_operator*>(method->getBody()->childAtIndex(0));
    ASSERT_OPERATOR(".", op);
    ASSERT_TYPE("Bar", op->getLeftOperand());
    ASSERT_METHOD_CALL("baz", op->getRightOperand());
}
