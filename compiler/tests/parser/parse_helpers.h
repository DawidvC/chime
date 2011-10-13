#include "compiler/compiler.h"

#define ASSERT_IMPORT(x) do { \
    ASSERT_EQ("import", x->nodeName()); \
    } while (0)

#define ASSERT_GLOBAL_VARIABLE(x, y) do { \
   ASSERT_EQ("Global Variable", y->nodeName()); \
   ASSERT_EQ(x, ((ast::GlobalVariable*)y)->getIdentifier()); \
   } while (0)

#define ASSERT_CLOSED_GLOBAL_VARIABLE(x, y) do { \
   ASSERT_EQ("Closed Global Variable", y->nodeName()); \
   ASSERT_EQ(x, ((ast::GlobalVariable*)y)->getIdentifier()); \
   } while (0)
   
#define ASSERT_LOCAL_VARIABLE(x, y) do { \
   ASSERT_EQ("Local Variable", y->nodeName()); \
   ASSERT_EQ(x, ((ast::LocalVariable*)y)->getIdentifier()); \
   } while (0)

#define ASSERT_CLOSED_LOCAL_VARIABLE(x, y) do { \
   ASSERT_EQ("Closed Local Variable", y->nodeName()); \
   ASSERT_EQ(x, ((ast::LocalVariable*)y)->getIdentifier()); \
   } while (0)

#define ASSERT_SHARED_LOCAL_VARIABLE(x, y) do { \
   ASSERT_EQ("Shared Local Variable", y->nodeName()); \
   ASSERT_EQ(x, ((ast::LocalVariable*)y)->getIdentifier()); \
   } while (0)

#define ASSERT_INSTANCE_VARIABLE(x, y) do { \
   ASSERT_EQ("Instance Variable", y->nodeName()); \
   ASSERT_EQ(x, ((ast::InstanceVariable*)y)->getIdentifier()); \
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
    ASSERT_EQ("method definition", y->nodeName()); \
    ASSERT_EQ(x, ((ast::method_definition*)y)->getIdentifier()); \
    } while (0)

#define ASSERT_PROPERTY_DEFINITION(x, y) do { \
    ASSERT_EQ("property definition", y->nodeName()); \
    ASSERT_EQ(x, ((ast::PropertyDefinition*)y)->getIdentifier()); \
    } while (0)

#define ASSERT_ATTRIBUTE(x, y) do { \
    ASSERT_EQ("attribute", y->nodeName()); \
    ASSERT_EQ(x, ((ast::Attribute*)y)->getIdentifier()); \
    } while (0)

#define ASSERT_METHOD_PARAMETER(w, x, y, z) do { \
    ASSERT_EQ("method parameter", ((ast::method_parameter*)z)->node_name()); \
    ASSERT_EQ(y, ((ast::method_parameter*)z)->identifier()); \
    if (w) ASSERT_TYPE((const char*)w, ((ast::method_parameter*)z)->type()); \
    if (x) ASSERT_EQ((const char *)x, ((ast::method_parameter*)z)->label()); \
    } while (0)

#define ASSERT_OPERATOR(x, y) do { \
    ASSERT_EQ("Binary Operator", y->nodeName()); \
    ASSERT_EQ(x, ((ast::binary_operator*)y)->identifier()); \
    } while (0)

#define ASSERT_GLOBAL_ASSIGNMENT(x) ASSERT_EQ("Global Variable Assignment Operator", x->nodeName())
#define ASSERT_INSTANCE_ASSIGNMENT(x) ASSERT_EQ("Instance Variable Assignment Operator", x->nodeName())
#define ASSERT_LOCAL_ASSIGNMENT(x) ASSERT_EQ("Local Variable Assignment Operator", x->nodeName())
#define ASSERT_CLOSED_ASSIGNMENT(x) ASSERT_EQ("Closed Local Variable Assignment Operator", x->nodeName())

#define ASSERT_INDEXER(x) ASSERT_EQ("Index Operator", x->nodeName())
    
#define ASSERT_METHOD_CALL(x, y) do { \
    ASSERT_EQ("Method Call", y->nodeName()); \
    ASSERT_EQ(x, ((ast::MethodCall*)y)->identifier()); \
    } while (0)

#define ASSERT_CLOSURE(x) ASSERT_EQ("Closure", x->nodeName())

#define ASSERT_LITERAL_TRUE(x) do { \
    ASSERT_EQ("boolean literal", x->node_name()); \
    ASSERT_EQ(1, ((ast::boolean_literal*)x)->value()); \
    } while (0)

#define ASSERT_LITERAL_FALSE(x) do { \
    ASSERT_EQ("boolean literal", x->node_name()); \
    ASSERT_EQ(0, ((ast::boolean_literal*)x)->value()); \
    } while (0)

#define ASSERT_LITERAL_SELF(x) ASSERT_EQ("self literal", x->node_name())
    
#define ASSERT_LITERAL_STRING(x, y) do { \
    ASSERT_EQ("string literal", y->nodeName()); \
    ASSERT_EQ(x, ((ast::string_literal*)y)->value()); \
    } while (0)
    
#define ASSERT_LITERAL_INTEGER(x, y) do { \
    ASSERT_EQ("Integer Literal", y->nodeName()); \
    ASSERT_EQ(x, ((ast::integer_literal*)y)->value()); \
    } while (0)

#define ASSERT_IF_STATEMENT(x) ASSERT_EQ("if statement", x->nodeName())

#define ASSERT_WHILE(x) ASSERT_EQ("While", x->nodeName())
#define ASSERT_UNTIL(x) ASSERT_EQ("Until", x->nodeName())
#define ASSERT_LOOP(x)  ASSERT_EQ("Loop", x->nodeName())
#define ASSERT_LOOP_UNTIL(x) ASSERT_EQ("Loop Until", x->nodeName())
#define ASSERT_LOOP_WHILE(x) ASSERT_EQ("Loop While", x->nodeName())
    
#define ASSERT_TRY(x) ASSERT_EQ("try", x->node_name())

#define ASSERT_CATCH(x) ASSERT_EQ("catch", x->node_name())

#define ASSERT_FINALLY(x) ASSERT_EQ("finally", x->node_name())
    
#define ASSERT_THROW_STATEMENT(x) ASSERT_EQ("throw", x->node_name())

#define ASSERT_NEXT(x) ASSERT_EQ("next statement", x->nodeName())

#define ASSERT_RETURN(x) ASSERT_EQ("return", x->nodeName())

#define ASSERT_BREAK(x) ASSERT_EQ("Break", x->nodeName())
