#include "import.h"
#include "compiler/parser/parser.h"
#include "compiler/codegen/code_generator.h"
#include "compiler/ast/primary/type_reference.h"
#include "compiler/ast/literals/string_literal.h"

namespace ast
{
    Import::Import(chime::parser& parser)
    {
        chime::token* t;
        
        // parse the import statement
        parser.next_token_value("import");
        
        t = parser.look_ahead();
        if (t->isString())
        {
            string_literal* string;
            
            string = new string_literal(&parser);
            
            this->setImportand(string);
            
            // this needs to be smart and figure out if it's currently
            // a structural element or not
            parser.addSourceDependency(string->value());
        }
        else if (t->isType())
        {
            type_reference* type;
            
            type = new ast::type_reference(&parser);
            
            this->setImportand(type);
            
            // again, the structural/non-structrual seperation needs to be made here
            parser.addBinaryDependency(type->identifier());
        }
        else
        {
            parser.addError("import must be followed by a string or type");
        }
        
        parser.advance_past_ending_tokens();
    }
    
    Import::~Import()
    {
    }
    
    std::string Import::nodeName(void) const
    {
        return std::string("import");
    }
    
    ast::node* Import::getImportand() const
    {
        return this->childAtIndex(0);
    }
    void Import::setImportand(ast::node* n)
    {
        this->addChild(n);
    }
    
    llvm::Value* Import::codegen(chime::code_generator& generator)
    {
        // The import statement is a weird beast.  If it appears as a structural
        // statement, it does not actually generate any code at all.  Instead,
        // it just instructs the compiler to either compile and/or link in another
        // binary.  This makes it possible to define your dependancies in terms
        // of code, instead of build system instructions/scripts.
        //
        // But, when encountered as a non-structural element, it must be executed at
        // runtime.  This is kind of like a dlopen call.
        //
        // For now, we'll just handle the static case.
        
        ast::node*  node;
        
        node = this->getImportand();
        
        if (node->nodeName().compare(std::string("type reference")) == 0)
        {
            ast::type_reference* typeRef;
            llvm::Value*         cStringPtr;
            
            typeRef    = static_cast<ast::type_reference*>(node);
            cStringPtr = generator.make_constant_string(typeRef->identifier());
            
            //generator.getImportedNamespaces()->push_back(->identifier());
            //generator.getRuntime()->callChimeRuntimeLoad(cStringPtr);
            generator.callModuleInitFunction(typeRef->identifier());
        }
        else if (node->nodeName().compare(std::string("string literal")) == 0)
        {
            std::string typeIdentifier;
            
            typeIdentifier = static_cast<string_literal*>(node)->value();
            
            generator.callModuleInitFunction(typeIdentifier);
        }
        else
        {
            assert(0 && "Currently, only type-reference and string imports are supported");
        }
        
        return NULL;
    }
}