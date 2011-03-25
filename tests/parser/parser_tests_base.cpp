#include "parser_tests_base.h"

void ParserTestsBase::SetUp()
{
    _last_node = NULL;
}
    
void ParserTestsBase::TearDown()
{
    if (_last_node)
        delete _last_node;
}

chime::lexer* ParserTestsBase::lex(const char* input)
{
    return new chime::stringlexer(input);
}

ast::node* ParserTestsBase::parse(const char* input)
{
    chime::lexer*  lexer;
    chime::parser* parser;
    
    lexer  = this->lex(input);
    parser = new chime::parser(lexer);
    
    _last_node = parser->parse();
    
    parser->print_errors();
    
    delete parser;
    delete lexer;
    
    return _last_node;
}

ast::implementation* ParserTestsBase::parse_implementation(const char *input)
{
    return (ast::implementation*)this->parse(input)->child_at_index(0);
}

ast::method_definition* ParserTestsBase::parse_method_def(const char *input)
{
    return (ast::method_definition*)this->parse(input)->child_at_index(0);
}

