#include "../../ast/ast.h"

void assert_import(const char* value, ast::node* node);
void assert_operator(const char* identifier, ast::node* op);
void assert_entity(const char* identifier, ast::node* node);
void assert_type(const char* identifier, ast::node* node);
void assert_implementation(const char* identifier, const char* superclass, ast::node* node);
