#include <cstdio>
#include <string>

#include "../lexer/file_lexer.h"
#include "../parser/parser.h"
#include "../ast/ast.h"

int main(int argc, char* argv[])
{
    chime::parser*    parser;
    chime::filelexer* lexer;
    ast::node*        node;
    
    lexer  = new chime::filelexer(argv[1]);
    parser = new chime::parser(lexer);
    
    node = ast::construct(parser);
    if (!node)
        return 1;
    
    fprintf(stdout, "%s\n", node->string_representation().c_str());
    
    return 0;
}
