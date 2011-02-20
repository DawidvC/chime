#include <gtest/gtest.h>
#include "../lexer/string_lexer.h"

class LexerTest : public testing::Test
{
protected:
	chime::stringlexer* lexer;
	
	virtual void SetUp()
	{
		lexer = NULL;
	}
	
	virtual void TearDown()
	{
		if (lexer)
			delete lexer;
	}
	
	void lex(const char *input)
	{
		lexer = new chime::stringlexer(input);
	}
	
	void ExpectTokens(const char *tokens[])
	{
		int i = 0;

		while(tokens[i] != NULL)
		{
			EXPECT_EQ(tokens[i],lexer->next_token()->value);
			i++;
		}
	}
};

#pragma mark token functions
TEST_F(LexerTest, LookAhead)
{
	lex("hello.world()");
	
	EXPECT_EQ("hello", lexer->look_ahead(1)->value);
	EXPECT_EQ("hello", lexer->look_ahead(1)->value);
	
	EXPECT_EQ(".", lexer->look_ahead(2)->value);
	EXPECT_EQ("world", lexer->look_ahead(3)->value);
	EXPECT_EQ("(", lexer->look_ahead(4)->value);
	EXPECT_EQ(")", lexer->look_ahead(5)->value);
	
	EXPECT_EQ("hello", lexer->next_token()->value);
}
TEST_F(LexerTest, LookAheadWithBlankInput)
{
	lex("");
	
	EXPECT_TRUE(NULL == lexer->look_ahead(1));
}
TEST_F(LexerTest, LookAheadAndNext)
{
	lex("foo.bar()");
	
	EXPECT_EQ("foo", lexer->look_ahead(1)->value);
	EXPECT_EQ("foo", lexer->next_token()->value);
	
	EXPECT_EQ(".", lexer->look_ahead(1)->value);
	EXPECT_EQ(".", lexer->next_token()->value);
}
TEST_F(LexerTest, LookAheadTwoAndNext)
{
	lex("foo.bar()");
	
	EXPECT_EQ("foo", lexer->look_ahead(1)->value);
	EXPECT_EQ(".", lexer->look_ahead(2)->value);
	EXPECT_EQ("foo", lexer->next_token()->value);
	EXPECT_EQ(".", lexer->look_ahead(1)->value);
}

#pragma mark basic definitions
TEST_F(LexerTest, SimpleContructor)
{
	lex("");
	
	EXPECT_FALSE(lexer == NULL);
}
TEST_F(LexerTest, SimpleNextToken)
{
	lex("class foo {}");

	const char *tokens[] = {"class", "foo", "{", "}", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, MethodWithNoParameters)
{
	lex("method foo() {}");
	
	const char *tokens[] = {"method", "foo", "(", ")", "{", "}", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, MethodWithOneParameter)
{
	lex("method foo(bar) {}");
	
	const char *tokens[] = {"method", "foo", "(", "bar", ")", "{", "}", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, MethodsWithMathSymbolNames)
{
	lex("method +() {}");
	
	const char *tokens1[] = {"method", "+", "(", ")", "{", "}", NULL};
	ExpectTokens(tokens1);
	
	lex("method -() {}");
	
	const char *tokens2[] = {"method", "-", "(", ")", "{", "}", NULL};
	ExpectTokens(tokens2);
	
	lex("method *() {}");
	
	const char *tokens3[] = {"method", "*", "(", ")", "{", "}", NULL};
	ExpectTokens(tokens3);
	
	lex("method /() {}");
	
	const char *tokens4[] = {"method", "/", "(", ")", "{", "}", NULL};
	ExpectTokens(tokens4);
}
TEST_F(LexerTest, MethodsWithCharsAndMathSymbolName)
{
	lex("method +aa+() {}");
	
	const char *tokens[] = {"method", "+", "aa", "+", "(", ")", "{", "}", NULL};
	ExpectTokens(tokens);
}

#pragma mark expressions
TEST_F(LexerTest, MultiplyExpression)
{
	lex("b = a * a");
	
	const char *tokens[] = {"b", "=", "a", "*", "a", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, MethodCall)
{
	lex("k = this_is(a:big method:call)");
	
	const char *tokens[] = {"k", "=", "this_is", "(", "a", ":", "big", "method", ":", "call", ")", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, FloatingPointExpression)
{
	lex("123.4567 + -2345.23452345");
	
	const char *tokens[] = {"123.4567", "+", "-", "2345.23452345", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, IntegerExpression)
{
	lex("123778 - 34234234");
	
	const char *tokens[] = {"123778", "-", "34234234", NULL};
	ExpectTokens(tokens);
}

#pragma mark whitespace handling
TEST_F(LexerTest, LeadingWhitespace)
{
	lex("   \n   \n      \n \t\t\t\t \nclass ABC {}");
	
	const char *tokens[] = {"class", "ABC", "{", "}", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, WhitespaceInDefinition)
{
	lex("method foo()\n{}");
	
	const char *tokens[] = {"method", "foo", "(", ")", "{", "}", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, SignificantNewlines)
{
	lex("a += b\na = 0");
	
	lexer->ignore_new_lines = false;
	
	const char *tokens[] = {"a", "+", "=", "b", "\n", "a", "=", "0", NULL};
	ExpectTokens(tokens);
	
	lexer->ignore_new_lines = true;
}

#pragma mark comments
TEST_F(LexerTest, LeadingInlineComments)
{
	lex("//this is a comment\n// and so is this\nmethod hello {}");
	
	const char *tokens[] = {"method", "hello", "{", "}", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, BlankInlineComments)
{
	lex("//\nmethod hello {}");
	
	const char *tokens[] = {"method", "hello", "{", "}", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, CommentsInMethodDefinition)
{
	lex("//this is a comment\nmethod // and so is this\n hello {}");
	
	const char *tokens[] = {"method", "hello", "{", "}", NULL};
	ExpectTokens(tokens);
}

#pragma mark literal handling
TEST_F(LexerTest, SimpleString)
{
	lex("Library.printf(\"hello world\")");
	
	const char *tokens[] = {"Library", ".", "printf", "(", "\"hello world\"", ")", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, MoreComplexString)
{
	lex("(\"This is a && ^ () {} much more {\n} complex string\")");
	
	const char *tokens[] = {"(","\"This is a && ^ () {} much more {\n} complex string\"", ")", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, SingleFloatingPoint)
{
	lex("5.667");
	
	const char *tokens[] = {"5.667", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, IntegerMethodCall)
{
	lex("5.times");
	
	const char *tokens[] = {"5", ".", "times", NULL};
	ExpectTokens(tokens);
}

TEST_F(LexerTest, FloatMethodCall)
{
	lex("5.6.floor");
	
	const char *tokens[] = {"5.6", ".", "floor", NULL};
	ExpectTokens(tokens);
}

#pragma mark parameters
TEST_F(LexerTest, Parameters)
{
	lex("foo(Integer bar:bas)");

	const char *tokens[] = {"foo", "(", "Integer","bar", ":", "bas", ")", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, TwoSimpleParameters)
{
	lex("foo(bar, bas)");

	const char *tokens[] = {"foo", "(", "bar", ",", "bas", ")", NULL};
	ExpectTokens(tokens);
}

#pragma mark operators
TEST_F(LexerTest, DotOperator)
{
	lex("x.hello");

	const char *tokens[] = {"x", ".", "hello", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, DoubleDot)
{
	lex("x..hello()");

	const char *tokens[] = {"x", ".", ".", "hello", "(", ")", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, AssignmentOperator)
{
	lex("x = y");

	const char *tokens[] = {"x", "=", "y", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, ComparisonOperator)
{
	lex("x == y");

	const char *tokens[] = {"x", "==", "y", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, LessOrEqualOperator)
{
	lex("x <= y");

	const char *tokens[] = {"x", "<=", "y", NULL};
	ExpectTokens(tokens);
}
TEST_F(LexerTest, GreaterOrEqualOperator)
{
	lex("x >= y");

	const char *tokens[] = {"x", ">=", "y", NULL};
	ExpectTokens(tokens);
}
