#include <gtest/gtest.h>
#include "../../frontend/chime.h"

class BasicParserTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        _last_node = NULL;
    }
    
    virtual void TearDown()
    {
        if (_last_node)
            delete _last_node;
    }
    
    ast::node* parse(const char *input)
    {
        chime::stringlexer* lexer;
        chime::parser*      parser;
        
        lexer  = new chime::stringlexer(input);
        parser = new chime::parser(lexer);
        
        _last_node = ast::construct(parser);
        
        delete parser;
        delete lexer;
        
        return _last_node;
    }
    
private:
    ast::node* _last_node;
};

TEST_F(BasicParserTest, ImportIdentifier)
{
    ast::node* node;
    
    node = parse("import Yo.Dog");
    
    ASSERT_EQ("import", node->node_name());
}
