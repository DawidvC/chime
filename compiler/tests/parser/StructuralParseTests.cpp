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
