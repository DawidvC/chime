#include "compiler/compiler.h"

#define ASSERT_IMPORT(x) do { \
    ASSERT_EQ("import", x->nodeName()); \
    } while (0)

#define ASSERT_GLOBAL_VARIABLE(x, y) do { \
   ASSERT_EQ("Global Variable", y->nodeName()); \
   ASSERT_EQ(x, ((chime::GlobalVariable*)y)->getIdentifier()); \
   } while (0)

#define ASSERT_CLOSED_GLOBAL_VARIABLE(x, y) do { \
   ASSERT_EQ("Closed Global Variable", y->nodeName()); \
   ASSERT_EQ(x, ((chime::GlobalVariable*)y)->getIdentifier()); \
   } while (0)
   
#define ASSERT_LOCAL_VARIABLE(x, y) do { \
   ASSERT_EQ("Local Variable", y->nodeName()); \
   ASSERT_EQ(x, ((chime::LocalVariable*)y)->getIdentifier()); \
   } while (0)

#define ASSERT_CLOSED_LOCAL_VARIABLE(x, y) do { \
   ASSERT_EQ("Closed Local Variable", y->nodeName()); \
   ASSERT_EQ(x, ((chime::LocalVariable*)y)->getIdentifier()); \
   } while (0)

#define ASSERT_SHARED_LOCAL_VARIABLE(x, y) do { \
   ASSERT_EQ("Shared Local Variable", y->nodeName()); \
   ASSERT_EQ(x, ((chime::LocalVariable*)y)->getIdentifier()); \
   } while (0)

#define ASSERT_INSTANCE_VARIABLE(x, y) do { \
   ASSERT_EQ("Instance Variable", y->nodeName()); \
   ASSERT_EQ(x, ((chime::InstanceVariable*)y)->getIdentifier()); \
   } while (0)

#define ASSERT_TYPE(x, y) do { \
    ASSERT_EQ("Type", y->nodeName()); \
    ASSERT_EQ(x, dynamic_cast<chime::Type*>(y)->identifier()); \
    } while (0)

#define ASSERT_IMPLEMENTATION(x, y, z) do { \
    ASSERT_EQ("implementation", z->node_name()); \
    ASSERT_TYPE(x, ((chime::Implementation*)z)->getTypeRef().get()); \
    } while (0)

#define ASSERT_TRAIT(x, y) do { \
    ASSERT_EQ("Trait", y->nodeName()); \
    ASSERT_TYPE(x, static_cast<chime::Trait*>(y)->getTypeRef().get()); \
    } while (0)

#define ASSERT_INCLUDE(x, y) do { \
    ASSERT_EQ("Include", y->nodeName()); \
    ASSERT_TYPE(x, static_cast<chime::Include*>(y)->getTraitType().get()); \
    } while (0)

#define ASSERT_CODE_BLOCK(x) ASSERT_EQ("code block", x->nodeName());

#define ASSERT_METHOD_DEFINITION(x, y) do { \
    ASSERT_EQ("method definition", y->nodeName()); \
    ASSERT_EQ(x, ((chime::method_definition*)y)->getIdentifier()); \
    } while (0)

#define ASSERT_PROPERTY_DEFINITION(x, y) do { \
    ASSERT_EQ("property definition", y->nodeName()); \
    ASSERT_EQ(x, ((chime::PropertyDefinition*)y)->getIdentifier()); \
    } while (0)

#define ASSERT_INDEXER_DEFINITION(x, y) do { \
    ASSERT_EQ("Indexer Definition", y->nodeName()); \
    ASSERT_EQ(x, static_cast<chime::IndexerDefinition*>(y)->getIdentifier()); \
    } while (0)

#define ASSERT_OPERATOR_DEFINITON(x, y) do { \
   ASSERT_EQ("Operator Definition", y->nodeName()); \
   ASSERT_EQ(x, static_cast<chime::OperatorDefinition*>(y)->getIdentifier()); \
   } while (0)

#define ASSERT_ATTRIBUTE(x, y) do { \
    ASSERT_EQ("attribute", y->nodeName()); \
    ASSERT_EQ(x, ((chime::Attribute*)y)->getIdentifier()); \
    } while (0)

#define ASSERT_METHOD_PARAMETER(w, x, y, z) do { \
    ASSERT_EQ("Parameter", dynamic_pointer_cast<chime::Parameter>(z)->nodeName()); \
    ASSERT_EQ(y, dynamic_pointer_cast<chime::Parameter>(z)->getIdentifier()); \
    if (w) ASSERT_TYPE((const char*)w, dynamic_pointer_cast<chime::Parameter>(z)->type().get()); \
    if (x) ASSERT_EQ((const char *)x, dynamic_pointer_cast<chime::Parameter>(z)->label()); \
    } while (0)

#define ASSERT_OPERATOR(x, y) do { \
    ASSERT_EQ("Binary Operator", y->nodeName()); \
    ASSERT_EQ(x, ((chime::binary_operator*)y)->identifier()); \
    } while (0)

#define ASSERT_AND_OPERATOR(x) ASSERT_EQ("And Operator", x->nodeName())
#define ASSERT_OR_OPERATOR(x) ASSERT_EQ("Or Operator", x->nodeName())

#define ASSERT_GLOBAL_ASSIGNMENT(x) ASSERT_EQ("Global Variable Assignment Operator", x->nodeName())
#define ASSERT_INSTANCE_ASSIGNMENT(x) ASSERT_EQ("Instance Variable Assignment Operator", x->nodeName())
#define ASSERT_LOCAL_ASSIGNMENT(x) ASSERT_EQ("Local Variable Assignment Operator", x->nodeName())
#define ASSERT_CLOSED_ASSIGNMENT(x) ASSERT_EQ("Closed Local Variable Assignment Operator", x->nodeName())

#define ASSERT_INDEXER(x) ASSERT_EQ("Index Operator", x->nodeName())

#define ASSERT_INDEXER_ASSIGNMENT(x, y) do { \
   ASSERT_EQ("Index Assignment", y->nodeName()); \
   ASSERT_EQ(x, static_cast<chime::IndexAssignmentOperator*>(y)->getIdentifier()); \
   } while (0)

#define ASSERT_METHOD_CALL(x, y) do { \
    ASSERT_EQ("Method Call", y->nodeName()); \
    ASSERT_EQ(x, ((chime::MethodCall*)y)->identifier()); \
    } while (0)

#define ASSERT_CLOSURE(x) ASSERT_EQ("Closure", x->nodeName())

#define ASSERT_LITERAL_TRUE(x) do { \
    ASSERT_EQ("boolean literal", x->node_name()); \
    ASSERT_EQ(1, ((chime::boolean_literal*)x)->value()); \
    } while (0)

#define ASSERT_LITERAL_FALSE(x) do { \
    ASSERT_EQ("boolean literal", x->node_name()); \
    ASSERT_EQ(0, ((chime::boolean_literal*)x)->value()); \
    } while (0)

#define ASSERT_LITERAL_SELF(x) ASSERT_EQ("self literal", x->node_name())

#define ASSERT_CLOSED_LITERAL_SELF(x) ASSERT_EQ("Closed Self Literal", x->nodeName())

#define ASSERT_LITERAL_STRING(x, y) do { \
    ASSERT_EQ("string literal", y->nodeName()); \
    ASSERT_EQ(x, ((chime::string_literal*)y)->value()); \
    } while (0)
    
#define ASSERT_LITERAL_INTEGER(x, y) do { \
    ASSERT_EQ("Integer Literal", y->nodeName()); \
    ASSERT_EQ(x, static_cast<chime::IntegerLiteral*>(y)->getValue()); \
    } while (0)

#define ASSERT_LITERAL_FLOAT(x, y) do { \
    ASSERT_EQ("Float Literal", y->nodeName()); \
    ASSERT_EQ(x, static_cast<chime::FloatLiteral*>(y)->getValue()); \
    } while (0)

#define ASSERT_LITERAL_IMAGINARY_INTEGER(x, y) do { \
    ASSERT_EQ("Imaginary Integer Literal", y->nodeName()); \
    ASSERT_EQ(x, static_cast<chime::ImaginaryIntegerLiteral*>(y)->getValue()); \
    } while (0)

#define ASSERT_LITERAL_RANGE(x, y, z) do { \
    ASSERT_EQ("Range Literal", z->nodeName()); \
    ASSERT_EQ(x, static_cast<chime::RangeLiteral*>(z)->getStartValue()); \
    ASSERT_EQ(y, static_cast<chime::RangeLiteral*>(z)->getEndValue()); \
    } while (0)

#define ASSERT_LITERAL_NIL(x) ASSERT_EQ("Nil Literal", x->nodeName())

#define ASSERT_LITERAL_ARRAY(x) ASSERT_EQ("Array Literal", x->nodeName())

#define ASSERT_LITERAL_HASH(x) ASSERT_EQ("Hash Literal", x->nodeName())

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

#define ASSERT_NEXT(x) ASSERT_EQ("Next", x->nodeName())

#define ASSERT_RETURN(x) ASSERT_EQ("return", x->nodeName())
#define ASSERT_SINGLE_VARIABLE_RETURN(x) ASSERT_EQ("Single Variable Return", x->nodeName())

#define ASSERT_BREAK(x) ASSERT_EQ("Break", x->nodeName())
