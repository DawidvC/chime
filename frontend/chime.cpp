#include <cstdio>
#include <string>
#include <getopt.h>

#include "../lexer/file_lexer.h"
#include "../parser/parser.h"
#include "../ast/ast.h"

void get_options(int argc, char* argv[])
{
    int32_t c;
    
    static struct option longopts[] = {
        { "help",       no_argument,            NULL,           'h' },
        { "print",      no_argument,            NULL,           'p' },
        { NULL,         0,                      NULL,           0 }
    };
    
    while ((c = getopt_long(argc, argv, "hp", longopts, NULL)) != -1)
    {
        switch (c)
        {
            case 'h':
            default:
                printf("usage: chime [options] [--] [file] [arguments]\n");
                printf("\n");
                printf("  -h (--help)   print this help message and exit\n");
                printf("  -p (--print)  print out the AST representation for the input file\n");
                exit(0);
                break;
        }
    }
    
    argc -= optind;
    argv += optind;
}

int main(int argc, char* argv[])
{
    chime::parser*    parser;
    chime::filelexer* lexer;
    ast::node*        node;
    
    get_options(argc, argv);
    
    lexer  = new chime::filelexer(argv[1]);
    parser = new chime::parser(lexer);
    
    node = parser->parse();
    if (!node)
        return 1;
    
    parser->print_errors();
    
    fprintf(stdout, "%s\n", node->string_representation().c_str());
    
    return 0;
}
