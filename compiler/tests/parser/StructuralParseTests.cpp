#include "parser_tests_base.h"

class StructuralParseTests : public ParserTestsBase
{
};

TEST_F(StructuralParseTests, TopLevelMethod)
{
    ast::Node* node;
    
    node = parse("method foo() {}");
    
    ASSERT_METHOD_DEFINITION("foo", node->childAtIndex(0));
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
