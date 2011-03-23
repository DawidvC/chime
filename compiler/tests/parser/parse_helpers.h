#include "../../ast/ast.h"

void assert_import(ast::node* node);
void assert_operator(const char* identifier, ast::node* op);
void assert_entity(const char* identifier, ast::node* node);
void assert_type(const char* identifier, ast::node* node);
void assert_implementation(const char* identifier, const char* superclass, ast::node* node);
void assert_variable_definition(const char* type, const char* identifier, ast::node* node);
void assert_method_definition(const char* identifier, ast::node* node);
void assert_method_parameter(const char* type, const char* label, const char* identifier, ast::method_parameter* node);
void assert_operator(const char* identifier, ast::basic_operator* op);
void assert_method_call(const char* identifier, ast::node* node);
void assert_literal(const char* value, ast::node* node);
void assert_block(ast::node* node);
void assert_next(ast::node* node);
void assert_if_statement(ast::node* node);