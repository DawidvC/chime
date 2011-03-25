#include "parser_tests_base.h"
#include "../../lexer/file_lexer.h"

class FileParserTests : public ParserTestsBase
{
    virtual chime::lexer* lex(const char* input)
    {
        chime::lexer* lexer;
        std::string   s;
        
        s += "/tmp/chime/tests/fixtures/";
        s += input;
        
        lexer = new chime::filelexer(s.c_str());
        
        return lexer;
    }
};

TEST_F(FileParserTests, BasicImplemenation)
{
    ast::node* node;
    
    node = this->parse("basic_implementation.chm");
    
    assert_implementation("Foo", "Bar", node->child_at_index(0));
    ASSERT_EQ(1, (int)node->child_count());
}