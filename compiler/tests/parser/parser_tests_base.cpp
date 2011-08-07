#include "parser_tests_base.h"
#include "compiler/lexer/string_lexer.h"

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
    
    //assert(parser->errors()->size() == 0 && "There should not be any errors after parsing");
    
    parser->print_errors();
    
    delete parser;
    delete lexer;
    
    return _last_node;
}

ast::Implementation* ParserTestsBase::parse_implementation(const char *input)
{
    return (ast::Implementation*)this->parse(input)->child_at_index(0);
}

ast::method_definition* ParserTestsBase::parse_method_def(const char *input)
{
    return (ast::method_definition*)this->parse(input)->child_at_index(0);
}

ast::method_call* ParserTestsBase::parse_method_call(const char* input)
{
    return dynamic_cast<ast::method_call*>(this->parse(input)->child_at_index(0));
}
