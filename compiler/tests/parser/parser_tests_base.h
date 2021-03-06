#ifndef PARSER_TEST_BASE
#define PARSER_TEST_BASE

#include "compiler/lexer/lexer.h"
#include "compiler/ast/ast.h"
#include "compiler/tests/parser/parse_helpers.h"
#include "compiler/parser/ParseError.h"

#include <gtest/gtest.h>

class ParserTestsBase : public testing::Test
{
protected:
    virtual void SetUp();
    
    virtual void TearDown();
    
    virtual chime::lexer* lex(const char* input);
    chime::Node*          parse(const char* input);
    
    template <typename T>
    T parseFirst(const char* input)
    {
        return static_cast<T>(this->parse(input)->childAtIndex(0));
    }
    
    chime::Implementation*    parse_implementation(const char* input);
    chime::method_definition* parse_method_def(const char* input);
    chime::MethodCall*        parseMethodCall(const char* input);
    chime::binary_operator*   parseOperator(const char* input);
    
    std::vector<chime::ParseErrorRef> getErrors() const;
    
protected:
    chime::Node*                      _last_node;
    std::vector<chime::ParseErrorRef> _errors;
};

#endif
