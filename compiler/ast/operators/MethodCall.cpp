#include "MethodCall.h"
#include "compiler/ast/structural/Closure.h"

namespace ast
{
    ast::Node* MethodCall::parse(chime::parser& parser)
    {
        chime::token* t;
        MethodCall*   call;
        
        call = new ast::MethodCall();
        
        call->identifier(parser.next_token_value());
        
        t = parser.look_ahead();
        if (t->equal_to("="))
        {
            parser.next_token_value("=");
            
            // we have a property setter
            call->identifier(call->identifier() + "=");
            
            call->addChild(parser.parse_expression());
        }
        else if (t->equal_to("("))
        {
            // ( arguments )
            MethodCall::parseArguments(parser, call);
        }
        else
        {
            assert(0 && "something funny happened while parsing a MethodCall");
        }
        
        return call;
    }
    
    void MethodCall::parseArguments(chime::parser& parser, ast::MethodCall* call)
    {
        chime::token* t;
        ast::node*    node;
        
        parser.next_token_value("(");
        
        while (true)
        {
            t = parser.look_ahead();
            
            if (t->empty() || t->equal_to(")"))
                break;
            
            // handle labels
            if (parser.look_ahead(2)->equal_to(":"))
            {
                parser.next_token();
                parser.next_token_value(":");
                if (parser.look_ahead()->equal_to(")"))
                {
                    //there needs to be a block after this
                    break;
                }
            }
            
            node = parser.parse_expression();
            if (node == NULL)
                break;
                
            call->addChild(node);
            
            // take care of the commas
            if (parser.look_ahead()->equal_to(","))
                parser.next_token_value(",");
        }
        
        parser.next_token_value(")");
        
        if (parser.look_ahead()->equal_to("do"))
        {
            call->addChild(ast::Closure::parse(parser));
        }
    }
    
    std::string MethodCall::nodeName(void) const
    {
        return std::string("Method Call");
    }
    
    std::string MethodCall::stringRepresentation(int depth) const
    {
        std::string str;
        
        str.append(depth*2, ' ');
        str.append("Method Call: ");
        str.append(this->identifier());
        
        return str;
    }
    
    llvm::Value* MethodCall::codegenWithTarget(chime::code_generator& generator, llvm::Value* target)
    {
        llvm::Value*                      methodNamePtr;
        std::vector<llvm::Value*>         arguments;
        std::vector<ast::node*>::iterator it;
        
        assert(target);
        
        for (it = this->children()->begin(); it != this->children()->end(); ++it)
        {
            llvm::LoadInst* objectLoad;
            llvm::Value*    argumentValue;
            
            argumentValue = (*it)->codegen(generator);
            assert(argumentValue);
            
            objectLoad = generator.builder()->CreateLoad(argumentValue, "loaded argument");
            
            arguments.push_back(objectLoad);
        }
        
        methodNamePtr = generator.getConstantString(this->identifier());
        
        return generator.getRuntime()->callChimeObjectInvoke(target, methodNamePtr, arguments);
    }
}
