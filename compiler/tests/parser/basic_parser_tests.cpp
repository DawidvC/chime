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
    
    node = (ast::import*)parse("import Yo.Dog")->child_at_index(0);
    
    assert_import(node);
    
    op_node = (ast::binary_operator*)node->expression();
    assert_operator(".", op_node);
    assert_entity("Yo", op_node->left_operand());
    assert_entity("Dog", op_node->right_operand());
}

TEST_F(BasicParserTest, MultipleImports)
{
    ast::node*   node;
    ast::import* import;
    
    node = parse("import Yo\nimport Sup\n");
    
    import = (ast::import*)node->child_at_index(0);
    assert_import(import);
    assert_entity("Yo", import->expression());
    
    import = (ast::import*)node->child_at_index(1);
    assert_import(import);
    assert_entity("Sup", import->expression());
}

TEST_F(BasicParserTest, SimpleImplementation)
{
    ast::node* node;
    
    node = parse("implementation SomeClass {}");
    
    assert_implementation("SomeClass", NULL, node->child_at_index(0));
}

TEST_F(BasicParserTest, ImplementationWithSuperClass)
{
    ast::node* node;
    
    node = parse("implementation SomeClass : SuperClass {}");
    
    assert_implementation("SomeClass", "SuperClass", node->child_at_index(0));
}
