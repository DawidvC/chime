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

chime::Node* ParserTestsBase::parse(const char* input)
{
    chime::lexer*  lexer;
    chime::parser* parser;
    
    lexer  = this->lex(input);
    parser = new chime::parser(lexer);
    
    _last_node = parser->parse();
    
    _errors = parser->getErrors();
    
    parser->printErrors();
    
    delete parser;
    delete lexer;
    
    return _last_node;
}

chime::Implementation* ParserTestsBase::parse_implementation(const char *input)
{
    return dynamic_cast<chime::Implementation*>(this->parse(input)->childAtIndex(0));
}

chime::method_definition* ParserTestsBase::parse_method_def(const char *input)
{
    return dynamic_cast<chime::method_definition*>(this->parse(input)->childAtIndex(0));
}

chime::MethodCall* ParserTestsBase::parseMethodCall(const char* input)
{
    return dynamic_cast<chime::MethodCall*>(this->parse(input)->childAtIndex(0));
}

chime::binary_operator* ParserTestsBase::parseOperator(const char* input)
{
    return dynamic_cast<chime::binary_operator*>(this->parse(input)->childAtIndex(0));
}

std::vector<chime::ParseErrorRef> ParserTestsBase::getErrors() const
{
    return _errors;
}
