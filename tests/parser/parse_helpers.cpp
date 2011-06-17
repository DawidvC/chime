#include <gtest/gtest.h>
#include "tests/parser/parse_helpers.h"

void assert_import(ast::node* node)
{
    ASSERT_EQ("import", node->node_name());
}

void assert_entity(const char* identifier, ast::node* node)
{
    ASSERT_EQ("entity reference", node->node_name());
    ASSERT_EQ(identifier, ((ast::entity_reference*)node)->identifier());
}

void assert_type(const char* identifier, ast::node* node)
{
    ASSERT_EQ("type reference", node->node_name());
    ASSERT_EQ(identifier, ((ast::type_reference*)node)->identifier());
}

void assert_implementation(const char* identifier, const char* superclass, ast::node* node)
{
    ASSERT_EQ("implementation", node->node_name());
    assert_type(identifier, ((ast::implementation*)node)->type_ref());
    
    if (superclass)
    {
        assert_type(superclass, ((ast::implementation*)node)->super_class());
    }
}

void assert_variable_definition(const char* type, const char* identifier, ast::node* node)
{
    ASSERT_EQ("variable definition", node->node_name());
    ASSERT_EQ(identifier, ((ast::variable_definition*)node)->identifier());
    assert_type(type, ((ast::variable_definition*)node)->variable_type());
}

void assert_method_definition(const char* identifier, ast::node* node)
{
    ASSERT_EQ("method definition", node->node_name());
    ASSERT_EQ(identifier, ((ast::method_definition*)node)->identifier());
}

void assert_method_parameter(const char* type, const char* label, const char* identifier, ast::method_parameter* node)
{
    ASSERT_EQ("method parameter", node->node_name());
    ASSERT_EQ(identifier, node->identifier());
    
    if (type)
    {
        assert_type(type, node->type());
    }
    
    if (label)
    {
        ASSERT_EQ(label, node->label());
    }
}

void assert_operator(const char* identifier, ast::node* node)
{
    ASSERT_EQ("binary operator", node->node_name());
    ASSERT_EQ(identifier, ((ast::binary_operator*)node)->identifier());
}

void assert_method_call(const char* identifier, ast::node* node)
{
    ASSERT_EQ("method call", node->node_name());
    ASSERT_EQ(identifier, ((ast::method_call*)node)->identifier());
}

void assert_literal(const char* value, ast::node* node)
{
    ASSERT_EQ("string literal", node->node_name());
    ASSERT_EQ(value, ((ast::string_literal*)node)->value());
}

void assert_literal(int value, ast::node* node)
{
    ASSERT_EQ("integer literal", node->node_name());
    ASSERT_EQ(value, ((ast::integer_literal*)node)->value());
}

void assert_next(ast::node* node)
{
    ASSERT_EQ("next statement", node->node_name());
}

