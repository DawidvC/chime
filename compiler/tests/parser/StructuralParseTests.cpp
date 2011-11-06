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
    
    method = parseFirst<chime::MethodDefinition*>("method foo(one:Bar baz) {}");
    
    ASSERT_METHOD_DEFINITION("foo", method);
    ASSERT_METHOD_PARAMETER("Bar", "one", "baz", method->getParameters()->childAtIndex(0));
}

TEST_F(StructuralParseTests, MethodWithTypedReturnAndOneTypedArgument)
{
    chime::MethodDefinition* method;
    
    method = parseFirst<chime::MethodDefinition*>("method foo(Bar; Baz a) { return a }");
    
    ASSERT_METHOD_DEFINITION("foo", method);
    ASSERT_METHOD_PARAMETER("Baz", NULL, "a", method->getParameters()->childAtIndex(0));
    ASSERT_TYPE("Bar", method->getParameters()->getReturnType().get());
}

TEST_F(StructuralParseTests, PropertyWithGetAndSet)
{
    ast::PropertyDefinition* property;
    chime::Return*           ret;
    
    property = parseFirst<ast::PropertyDefinition*>("property foo(value) { get { return a } set { a = value } }");
    
    ASSERT_PROPERTY_DEFINITION("foo", property);
    ASSERT_METHOD_PARAMETER(NULL, NULL, "value", property->getParameters()->childAtIndex(0));
    
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
    ASSERT_RETURN(ret);
    ASSERT_LOCAL_VARIABLE("i", ret->getReturnValue().get());
}

TEST_F(StructuralParseTests, LabelledIndexerWithGet)
{
    chime::IndexerDefinition* indexer;
    chime::Return*            ret;
    
    indexer = parseFirst<chime::IndexerDefinition*>("indexer foo[i] { get { return a } }");
    
    ASSERT_INDEXER_DEFINITION("foo[]", indexer);
    
    ret = static_cast<chime::Return*>(indexer->getGetBody()->childAtIndex(0)); 
    ASSERT_RETURN(ret);
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
