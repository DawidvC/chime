#include <gtest/gtest.h>
#include "parse_helpers.h"

void assert_import(const char* value, ast::node* node)
{
    ASSERT_EQ("import", node->node_name());
    assert_type(value,  ((ast::import*)node)->importand());
}

void assert_operator(const char* identifier, ast::node* op)
{
    ASSERT_EQ(identifier, ((ast::basic_operator*)op)->identifier());
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
