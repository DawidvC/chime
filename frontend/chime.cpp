#include <cstdio>
#include <string>
#include <getopt.h>
#include <libgen.h>

#include "lexer/file_lexer.h"
#include "parser/parser.h"
#include "operations/code_generator.h"
#include "ast/ast.h"

#include <llvm/Pass.h>
#include <llvm/PassManager.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/Support/FormattedStream.h>

bool        print_ast;
bool        emit_llvm_ir;
const char* output_file_name;

// std::string path_with_new_extension(std::string path, const char* extension)
// {
//     path = path.substr(0, path.find_last_of("."));
//     
//     return path + extension;
// }

void get_options(int argc, char* argv[])
{
    int32_t c;
    
    static struct option longopts[] = {
        { "emit-llvm",  no_argument,            NULL,           'e' },
        { "help",       no_argument,            NULL,           'h' },
        { "output",     required_argument, NULL, 'o' },
        { "print",      no_argument,            NULL,           'p' },
        { NULL,         0,                      NULL,           0 }
    };
    
    print_ast        = false;
    emit_llvm_ir     = false;
    output_file_name = NULL;
    
    while ((c = getopt_long(argc, argv, "eho:p", longopts, NULL)) != -1)
    {
        switch (c)
        {
            case 'e':
                emit_llvm_ir = true;
                break;
            case 'o':
                output_file_name = optarg;
                break;
            case 'p':
                print_ast = true;
                break;
            case 'h':
            default:
                printf("usage: chime [options] [--] [file] [arguments]\n");
                printf("\n");
                printf("  -e (--emit-llvm)  create llvm IR\n");
                printf("  -h (--help)       print this help message and exit\n");
                printf("  -p (--print)      print out the AST representation for the input file\n");
                exit(0);
                break;
        }
    }
}

int main(int argc, char* argv[])
{
    chime::parser*         parser;
    chime::filelexer*      lexer;
    ast::node*             node;
    chime::code_generator* generator;
    
    get_options(argc, argv);
    argc -= optind;
    argv += optind;
    
    lexer     = new chime::filelexer(argv[0]);
    parser    = new chime::parser(lexer);
    generator = new chime::code_generator();
    
    node = parser->parse();
    if (!node)
        return 1;
    
    parser->print_errors();
    
    if (print_ast)
    {
        fprintf(stdout, "%s\n", node->string_representation().c_str());
        return 0;
    }
    
    generator->generate(node, "my module");
    
    if (emit_llvm_ir)
    {
        llvm::PassManager    pass_manager;
        std::string          error_string;
        llvm::raw_ostream*   stream;
        
        if (output_file_name)
        {
            stream = new llvm::raw_fd_ostream(output_file_name, error_string, 0);
        }
        else
        {
            stream = &llvm::outs();
        }   
        
        pass_manager.add(llvm::createPrintModulePass(stream));
        pass_manager.run(*generator->module());
    }
    
    return 0;
}
