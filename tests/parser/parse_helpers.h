#include "../../ast/ast.h"

void assert_import(ast::node* node);
void assert_entity(const char* identifier, ast::node* node);
void assert_type(const char* identifier, ast::node* node);
void assert_implementation(const char* identifier, const char* superclass, ast::node* node);
void assert_variable_definition(const char* type, const char* identifier, ast::node* node);
void assert_method_definition(const char* identifier, ast::node* node);
void assert_method_parameter(const char* type, const char* label, const char* identifier, ast::method_parameter* node);
void assert_operator(const char* identifier, ast::node* node);
void assert_method_call(const char* identifier, ast::node* node);
void assert_literal(const char* value, ast::node* node);
void assert_literal(int value, ast::node* node);
void assert_block(ast::node* node);
void assert_next(ast::node* node);
void assert_if_statement(ast::node* node);

#define ASSERT_ENTITY(x, y) do { \
    ASSERT_EQ("entity reference", y->node_name()); \
    ASSERT_EQ(x, ((ast::entity_reference*)y)->identifier()); \
    } while (0)
    
#define ASSERT_TYPE(x, y) do { \
    ASSERT_EQ("type reference", y->node_name()); \
    ASSERT_EQ(x, ((ast::type_reference*)y)->identifier()); \
    } while (0)
    
#define ASSERT_OPERATOR(x, y) do { \
    ASSERT_EQ("binary operator", y->node_name()); \
    ASSERT_EQ(x, ((ast::binary_operator*)y)->identifier()); \
    } while (0)

#define ASSERT_METHOD_CALL(x, y) do { \
    ASSERT_EQ("method call", y->node_name()); \
    ASSERT_EQ(x, ((ast::method_call*)y)->identifier()); \
    } while (0)

#define ASSERT_BLOCK(x) ASSERT_EQ("block", x->node_name())
