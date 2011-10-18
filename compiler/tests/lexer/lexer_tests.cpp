#include "compiler/lexer/string_lexer.h"
#ifndef USING_PREFIX_HEADERS
#    include <gtest/gtest.h>
#endif

class LexerTest : public testing::Test
{
protected:
    chime::LexerRef lexer;
    
    void lex(const char *input)
    {
        lexer = chime::LexerRef(new chime::stringlexer(input));
    }
    
	void ExpectTokens(const char *tokens[])
	{
		int i = 0;

		while(tokens[i] != NULL)
		{
            ASSERT_EQ(tokens[i], next_token_value());
			i++;
		}
	}
    
    chime::token* nextToken()
    {
        return lexer->next_token();
    }
    chime::token* next_token(void)
    {
        return this->nextToken();
    }
    std::string nextTokenValue()
    {
        return lexer->next_token()->value();
    }
    std::string next_token_value(void)
    {
        return this->nextTokenValue();
    }
};

#define ASSERT_TOKENS(t) do { \
    for (int i = 0; tokens[i] != NULL; ++i) \
    { \
        ASSERT_EQ(tokens[i], this->nextTokenValue()); \
    } \
} while (0);

#pragma mark token functions
TEST_F(LexerTest, LookAhead)
{
    lex("hello.world()");
    
    ASSERT_EQ("hello", lexer->look_ahead(1)->value());
    ASSERT_EQ("hello", lexer->look_ahead(1)->value());
    
    ASSERT_EQ(".",     lexer->look_ahead(2)->value());
    ASSERT_EQ("world", lexer->look_ahead(3)->value());
    ASSERT_EQ("(",     lexer->look_ahead(4)->value());
    ASSERT_EQ(")",     lexer->look_ahead(5)->value());
    
    ASSERT_EQ("hello", lexer->next_token()->value());
}
TEST_F(LexerTest, LookAheadWithBlankInput)
{
    lex("");
    
    ASSERT_TRUE(lexer->look_ahead(1)->empty());
    ASSERT_TRUE(lexer->look_ahead(1)->isPunctuation());
}
TEST_F(LexerTest, LookAheadAndNext)
{
    lex("foo.bar()");
    
    ASSERT_EQ("foo", lexer->look_ahead(1)->value());
    ASSERT_EQ("foo", lexer->next_token()->value());
    
    ASSERT_EQ(".",   lexer->look_ahead(1)->value());
    ASSERT_EQ(".",   lexer->next_token()->value());
}
TEST_F(LexerTest, LookAheadTwoAndNext)
{
    lex("foo.bar()");
    
    ASSERT_EQ("foo", lexer->look_ahead(1)->value());
    ASSERT_EQ(".",   lexer->look_ahead(2)->value());
    ASSERT_EQ("foo", lexer->next_token()->value());
    ASSERT_EQ(".",   lexer->look_ahead(1)->value());
}

TEST_F(LexerTest, LookAheadPastInput)
{
    lex("foo");
    
    EXPECT_TRUE(lexer->look_ahead(2)->empty());
    EXPECT_TRUE(lexer->look_ahead(3)->empty());
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
	
	const char *tokens[] = {"123.4567", "+", "-2345.23452345", NULL};
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
    
    lexer->ignore_new_lines(false);
    
    const char *tokens[] = {"a", "+=", "b", "\n", "a", "=", "0", NULL};
    ExpectTokens(tokens);
    
    lexer->ignore_new_lines(true);
}
TEST_F(LexerTest, NewlineAfterComment)
{
    lex("something // a comment\n");
    
    lexer->ignore_new_lines(false);
    
    const char *tokens[] = {"something", "\n", NULL};
    ExpectTokens(tokens);
    
    lexer->ignore_new_lines(true);
}
TEST_F(LexerTest, LineAccounting)
{
    lex("\na\n\n\n5 == 1\n");
    
    lexer->ignore_new_lines(false);
    
    ASSERT_EQ(1, lexer->current_line());
    
    ASSERT_EQ("\n",lexer->next_token()->value());
    ASSERT_EQ(2,   lexer->current_line());
    
    ASSERT_EQ("a",lexer->next_token()->value());
    ASSERT_EQ(2,   lexer->current_line());
    
    ASSERT_EQ("\n",lexer->next_token()->value());
    ASSERT_EQ(3,   lexer->current_line());
    
    ASSERT_EQ("\n",lexer->next_token()->value());
    ASSERT_EQ(4,   lexer->current_line());
    
    ASSERT_EQ("\n",lexer->next_token()->value());
    ASSERT_EQ(5,   lexer->current_line());
    
    ASSERT_EQ("5",lexer->next_token()->value());
    ASSERT_EQ(5,   lexer->current_line());
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

// *** literal handling
TEST_F(LexerTest, SimpleString)
{
    lex("Library.printf(\"hello world\")");
    
    const char *tokens[] = {"Library", ".", "printf", "(", "\"hello world\"", ")", NULL};
    ExpectTokens(tokens);
}

TEST_F(LexerTest, MoreComplexString)
{
    lex("(\"This is a && ^ () much more [\n] complex string\")");
    
    const char *tokens[] = {"(","\"This is a && ^ () much more [\n] complex string\"", ")", NULL};
    ExpectTokens(tokens);
}

TEST_F(LexerTest, InterpolatedString)
{
    lex("\"1 + 1 = {1 + 1}\"");
    
    const char* tokens[] = {"\"1 + 1 = \"{", "1", "+", "1", "}\"\"", NULL};
    ExpectTokens(tokens);
}

TEST_F(LexerTest, MoreComplexInterpolatedString)
{
    lex("\"1 + 1 = {2} = {2}\"");
    
    const char* tokens[] = {"\"1 + 1 = \"{", "2", "}\" = \"{", "2", "}\"\"", NULL};
    ExpectTokens(tokens);
}

TEST_F(LexerTest, UnterminatedString)
{
    lex("\"1 2 3");
    
    const char* tokens[] = {"\"1 2 3", NULL};
    ExpectTokens(tokens);
}

// TEST_F(LexerTest, MethodCallInInterpolatedString)
// {
//     lex("\"1 {2.to_string()} 3\"");
//     
//     const char* tokens[] = {"\"1 \"{", "2", ".", "to_string", "(", ")", "}\" 3\"", NULL};
//     ExpectTokens(tokens);
// }

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

TEST_F(LexerTest, NegativeInteger)
{
    lex("6 + -7");
    
    const char *tokens[] = {"6", "+", "-7", NULL};
    ExpectTokens(tokens);
}

TEST_F(LexerTest, HexadecimalInteger)
{
    lex("0x007");
    
    const char *tokens[] = {"0x007", NULL};
    ASSERT_TOKENS(tokens);
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
TEST_F(LexerTest, PlusEqualsOperator)
{
    lex("x += y");
    
    const char *tokens[] = {"x", "+=", "y", NULL};
    ASSERT_TOKENS(tokens);
}
TEST_F(LexerTest, DivideEqualsOperator)
{
    lex("x /= y");
    
    const char *tokens[] = {"x", "/=", "y", NULL};
    ASSERT_TOKENS(tokens);
}
TEST_F(LexerTest, MultiplyEqualsOperator)
{
    lex("x *= y");
    
    const char *tokens[] = {"x", "*=", "y", NULL};
    ASSERT_TOKENS(tokens);
}
TEST_F(LexerTest, MinusEqualsOperator)
{
    lex("x -= y");
    
    const char *tokens[] = {"x", "-=", "y", NULL};
    ASSERT_TOKENS(tokens);
}
TEST_F(LexerTest, SpaceshipOperator)
{
    lex("x <=> y");
    
    const char *tokens[] = {"x", "<=>", "y", NULL};
    ASSERT_TOKENS(tokens);
}
TEST_F(LexerTest, IndexerMethodCall)
{
    lex("foo[123]");
    
    const char *tokens[] = {"foo", "[", "123", "]", NULL};
    ASSERT_TOKENS(tokens);
}

TEST_F(LexerTest, RangeLiteral)
{
    lex("1:23");
    
    const char *tokens[] = {"1", ":", "23", NULL};
    ASSERT_TOKENS(tokens);
}
