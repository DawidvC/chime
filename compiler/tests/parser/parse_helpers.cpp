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

void assert_operator(const char* identifier, ast::basic_operator* op)
{
    ASSERT_EQ(identifier, op->identifier());
}
