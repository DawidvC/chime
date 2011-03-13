#include <gtest/gtest.h>
#include "../../frontend/chime.h"
#include "../../ast/ast.h"
#include "parse_helpers.h"

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
    ast::import*          node;
    ast::binary_operator* op_node;
    
    node = (ast::import*)parse("import Yo.Dog");
    
    assert_import(node);
    
    op_node = (ast::binary_operator*)node->expression();
    assert_operator(".", op_node);
    assert_entity("Yo", op_node->left_operand());
    assert_entity("Dog", op_node->right_operand());
}
