#ifndef USING_PREFIX_HEADERS
#    include "compiler/ast/ast.h"
#endif

#define ASSERT_IMPORT(x) do { \
    ASSERT_EQ("import", x->nodeName()); \
    } while (0)

#define ASSERT_ENTITY(x, y) do { \
    ASSERT_EQ("entity reference", y->node_name()); \
    ASSERT_EQ(x, ((ast::entity_reference*)y)->identifier()); \
    } while (0)
    
#define ASSERT_TYPE(x, y) do { \
    ASSERT_EQ("type reference", y->nodeName()); \
    ASSERT_EQ(x, ((ast::type_reference*)y)->identifier()); \
    } while (0)

#define ASSERT_IMPLEMENTATION(x, y, z) do { \
    ASSERT_EQ("implementation", z->node_name()); \
    ASSERT_TYPE(x, ((ast::Implementation*)z)->getTypeRef().get()); \
    } while (0)
    
#define ASSERT_CODE_BLOCK(x) ASSERT_EQ("code block", x->nodeName());

#define ASSERT_METHOD_DEFINITION(x, y) do { \
    ASSERT_EQ("method definition", y->node_name()); \
    ASSERT_EQ(x, ((ast::method_definition*)y)->identifier()); \
    } while (0)

#define ASSERT_PROPERTY_DEFINITION(x, y) do { \
    ASSERT_EQ("property definition", y->nodeName()); \
    ASSERT_EQ(x, ((ast::PropertyDefinition*)y)->getIdentifier()); \
    } while (0)

#define ASSERT_VARIABLE_DEFINITION(x, y, z) do { \
    ASSERT_EQ("variable definition", z->node_name()); \
    ASSERT_EQ(y, ((ast::variable_definition*)z)->identifier()); \
    ASSERT_TYPE(x, ((ast::variable_definition*)z)->variable_type()); \
    } while (0)

#define ASSERT_METHOD_PARAMETER(w, x, y, z) do { \
    ASSERT_EQ("method parameter", ((ast::method_parameter*)z)->node_name()); \
    ASSERT_EQ(y, ((ast::method_parameter*)z)->identifier()); \
    if (w) ASSERT_TYPE((const char*)w, ((ast::method_parameter*)z)->type()); \
    if (x) ASSERT_EQ((const char *)x, ((ast::method_parameter*)z)->label()); \
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

#define ASSERT_LITERAL_TRUE(x) do { \
    ASSERT_EQ("boolean literal", x->node_name()); \
    ASSERT_EQ(1, ((ast::boolean_literal*)x)->value()); \
    } while (0)

#define ASSERT_LITERAL_FALSE(x) do { \
    ASSERT_EQ("boolean literal", x->node_name()); \
    ASSERT_EQ(0, ((ast::boolean_literal*)x)->value()); \
    } while (0)

#define ASSERT_LITERAL_STRING(x, y) do { \
    ASSERT_EQ("string literal", y->node_name()); \
    ASSERT_EQ(x, ((ast::string_literal*)y)->value()); \
    } while (0)

#define ASSERT_LITERAL_INTEGER(x, y) do { \
    ASSERT_EQ("integer literal", y->node_name()); \
    ASSERT_EQ(x, ((ast::integer_literal*)y)->value()); \
    } while (0)

#define ASSERT_IF_STATEMENT(x) do { \
    ASSERT_EQ("if statement", x->nodeName()); \
    } while (0)

#define ASSERT_TRY(x) do { \
    ASSERT_EQ("try", x->node_name()); \
    } while (0)

#define ASSERT_CATCH(x) do { \
    ASSERT_EQ("catch", x->node_name()); \
    } while (0)

#define ASSERT_FINALLY(x) do { \
    ASSERT_EQ("finally", x->node_name()); \
    } while (0)
    
#define ASSERT_THROW_STATEMENT(x) do { \
    ASSERT_EQ("throw", x->node_name()); \
    } while (0)

#define ASSERT_NEXT(x) do { \
    ASSERT_EQ("next statement", x->nodeName()); \
    } while (0)

#define ASSERT_RETURN(x) do { \
    ASSERT_EQ("return", x->nodeName()); \
    } while (0)
