#include "parser_tests_base.h"

class StructuralParseTests : public ParserTestsBase
{
};

TEST_F(StructuralParseTests, Method)
{
    chime::MethodDefinition* node;
    
    node = parseFirst<chime::MethodDefinition*>("method foo() {}");
    
    ASSERT_METHOD_DEFINITION("foo", node);
    ASSERT_TRUE(node->isInstance());
}

TEST_F(StructuralParseTests, MethodDefinitionWithOneParameter)
{
    chime::MethodDefinition* method;
    
    method = parseFirst<chime::MethodDefinition*>("method new(arg1) { }");
    
    ASSERT_METHOD_DEFINITION("new:", method);
    ASSERT_METHOD_PARAMETER(NULL, NULL, "arg1", method->getParameters()[0]);
}

TEST_F(StructuralParseTests, MethodDefinitionWithOneTypedParameter)
{
    chime::MethodDefinition* method;
    
    method = parseFirst<chime::MethodDefinition*>("method new(Hash arg1) { }");
    
    ASSERT_METHOD_DEFINITION("new:", method);
    ASSERT_METHOD_PARAMETER("Hash", NULL, "arg1", method->getParameters()[0]);
}

TEST_F(StructuralParseTests, MethodDefinitionWithTwoParameters)
{
    chime::MethodDefinition* method;
    
    method = parseFirst<chime::MethodDefinition*>("method new(arg1, arg2) { }");
    
    ASSERT_METHOD_DEFINITION("new::", method);
    ASSERT_METHOD_PARAMETER(NULL, NULL, "arg1", method->getParameters()[0]);
    ASSERT_METHOD_PARAMETER(NULL, NULL, "arg2", method->getParameters()[1]);
}

TEST_F(StructuralParseTests, MethodDefinitionWithFunction)
{
    chime::MethodDefinition* method;
    
    method = parseFirst<chime::MethodDefinition*>("method foo(a, label:Function(data) block) {}");
    
    ASSERT_METHOD_DEFINITION("foo:label:", method);
    ASSERT_METHOD_PARAMETER(NULL,       NULL, "a",     method->getParameters()[0]);
    ASSERT_METHOD_PARAMETER("Function", NULL, "block", method->getParameters()[1]);
}

TEST_F(StructuralParseTests, ClassMethod)
{
    chime::MethodDefinition* node;
    
    node = parseFirst<chime::MethodDefinition*>("class method foo() {}");
    
    ASSERT_METHOD_DEFINITION("foo", node);
    ASSERT_FALSE(node->isInstance());
}

TEST_F(StructuralParseTests, MethodParameter)
{
    chime::MethodDefinition* method;
    
    method = parseFirst<chime::MethodDefinition*>("method foo(Bar baz) {}");
    
    ASSERT_METHOD_DEFINITION("foo:", method);
    ASSERT_METHOD_PARAMETER("Bar", NULL, "baz", method->getParameters()[0]);
}

TEST_F(StructuralParseTests, MethodWithTypedReturnAndOneTypedArgument)
{
    chime::MethodDefinition* method;
    
    method = parseFirst<chime::MethodDefinition*>("method foo(Bar; Baz a) { return a }");
    
    ASSERT_METHOD_DEFINITION("foo:", method);
    ASSERT_METHOD_PARAMETER("Baz", NULL, "a", method->getParameters()[0]);
    //ASSERT_TYPE("Bar", method->getParameters()[0]->type().get());
}

TEST_F(StructuralParseTests, PropertyWithGetAndSet)
{
    ast::PropertyDefinition* property;
    chime::Return*           ret;
    
    property = parseFirst<ast::PropertyDefinition*>("property foo(value) { get { return a } set { a = value } }");
    
    ASSERT_PROPERTY_DEFINITION("foo", property);
    ASSERT_METHOD_PARAMETER(NULL, NULL, "value", property->getParameters()[0]);
    
    ret = static_cast<chime::Return*>(property->getGetBody()->childAtIndex(0));
    ASSERT_LOCAL_VARIABLE("a", ret->getReturnValue().get());
}

TEST_F(StructuralParseTests, PropertyWithGetMissingParentheses)
{
    ast::PropertyDefinition* property;
    chime::Return*           ret;
    
    property = parseFirst<ast::PropertyDefinition*>("property foo { get { return a } }");
    
    ASSERT_PROPERTY_DEFINITION("foo", property);
    
    ret = static_cast<chime::Return*>(property->getGetBody()->childAtIndex(0));
    ASSERT_LOCAL_VARIABLE("a", ret->getReturnValue().get());
    
    ASSERT_FALSE(property->getSetBody());
}

TEST_F(StructuralParseTests, PropertyWithEqualsSyntax)
{
    chime::MethodDefinition* method;
    
    method = parseFirst<chime::MethodDefinition*>("method foo=(bar) { }");
    
    ASSERT_METHOD_DEFINITION("foo=:", method);
    ASSERT_METHOD_PARAMETER(NULL, NULL, "bar", method->getParameters()[0]);
}

TEST_F(StructuralParseTests, AttributeAccessedInGetProperty)
{
    ast::Implementation*     implemenation;
    ast::PropertyDefinition* property;
    chime::Return*           ret;
    
    implemenation = parse_implementation("implementation Foo\n { attribute a\n property foo(value) { get { return a } } }");
    
    ASSERT_IMPLEMENTATION("Foo", NULL, implemenation);
    
    property = static_cast<ast::PropertyDefinition*>(implemenation->getBody()->childAtIndex(1));
    
    ASSERT_PROPERTY_DEFINITION("foo", property);
    
    ret = static_cast<chime::Return*>(property->getGetBody()->childAtIndex(0));
    ASSERT_INSTANCE_VARIABLE("a", ret->getReturnValue().get());
}

TEST_F(StructuralParseTests, AttributeAccessedInSetProperty)
{
    ast::Implementation*     implemenation;
    ast::PropertyDefinition* property;
    ast::BinaryOperator*     op;
    
    implemenation = parse_implementation("implementation Foo\n { attribute a\n property foo(value) { set { a = value } } }");
    ASSERT_IMPLEMENTATION("Foo", NULL, implemenation);
    
    property = static_cast<ast::PropertyDefinition*>(implemenation->getBody()->childAtIndex(1));
    ASSERT_PROPERTY_DEFINITION("foo", property);
    
    op = static_cast<ast::binary_operator*>(property->getSetBody()->childAtIndex(0));
    ASSERT_INSTANCE_ASSIGNMENT(op);
    ASSERT_INSTANCE_VARIABLE("a", op->getLeftOperand());
    ASSERT_LOCAL_VARIABLE("value", op->getRightOperand());
}

TEST_F(StructuralParseTests, SelfIndexerWithGetAndSet)
{
    chime::IndexerDefinition* indexer;
    chime::Return*            ret;
    
    indexer = parseFirst<chime::IndexerDefinition*>("indexer self[i](value) { get { return i } set { value.print() } }");
    
    ASSERT_INDEXER_DEFINITION("[]", indexer);
    
    ret = static_cast<chime::Return*>(indexer->getGetBody()->childAtIndex(0)); 
    ASSERT_SINGLE_VARIABLE_RETURN(ret);
    ASSERT_LOCAL_VARIABLE("i", ret->getReturnValue().get());
}

TEST_F(StructuralParseTests, LabelledIndexerWithGet)
{
    chime::IndexerDefinition* indexer;
    chime::Return*            ret;
    
    indexer = parseFirst<chime::IndexerDefinition*>("indexer foo[i] { get { return a } }");
    
    ASSERT_INDEXER_DEFINITION("foo[]", indexer);
    
    ret = static_cast<chime::Return*>(indexer->getGetBody()->childAtIndex(0)); 
    ASSERT_SINGLE_VARIABLE_RETURN(ret);
    ASSERT_LOCAL_VARIABLE("a", ret->getReturnValue().get());
}

TEST_F(StructuralParseTests, OperatorDefinition)
{
    chime::OperatorDefinition* operatorDef;
    
    operatorDef = parseFirst<chime::OperatorDefinition*>("operator -(value) { }");
    
    ASSERT_OPERATOR_DEFINITON("-", operatorDef);
}

TEST_F(StructuralParseTests, TraitDefinition)
{
    chime::Trait* trait;
    
    trait = parseFirst<chime::Trait*>("trait Nothing { }");
    
    ASSERT_TRAIT("Nothing", trait);
}

TEST_F(StructuralParseTests, IncludeInImplementation)
{
    ast::Implementation* implementation;
    
    implementation = parseFirst<ast::Implementation*>("implementation Foo { include Bar }");
    
    ASSERT_IMPLEMENTATION("Foo", NULL, implementation);
    ASSERT_INCLUDE("Bar", implementation->getBody()->childAtIndex(0));
}

TEST_F(StructuralParseTests, ImplementationMethodWithParameterUsedInBody)
{
    ast::Implementation*    implementation;
    ast::method_definition* method;
    ast::binary_operator*   op;
    
    implementation = parseFirst<ast::Implementation*>("implementation Foo\n { attribute a\n method foo(value) { a = value } }");
    ASSERT_IMPLEMENTATION("Foo", NULL, implementation);
    
    method = static_cast<ast::method_definition*>(implementation->getBody()->childAtIndex(1));
    ASSERT_METHOD_DEFINITION("foo:", method);
    
    op = static_cast<ast::binary_operator*>(method->getBody()->childAtIndex(0));
    ASSERT_INSTANCE_ASSIGNMENT(op);
    ASSERT_INSTANCE_VARIABLE("a", op->getLeftOperand());
    ASSERT_LOCAL_VARIABLE("value", op->getRightOperand());
}
