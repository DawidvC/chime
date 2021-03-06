#include "MethodCall.h"
#include "compiler/ast/structural/Closure.h"
#include "compiler/ast/operators/IndexOperator.h"

namespace chime
{
    Node* MethodCall::parse(chime::parser& parser)
    {
        chime::token* t;
        MethodCall*   call;
        
        call = new MethodCall();
        
        call->identifier(parser.nextTokenValue());
        
        t = parser.look_ahead();
        if (t->equal_to("="))
        {
            parser.nextTokenValue("=");
            
            // we have a property setter
            call->identifier(call->identifier() + "=:");
            
            call->addChild(parser.parse_expression());
        }
        else if (t->equal_to("("))
        {
            // ( arguments )
            MethodCall::parseArguments(parser, call);
        }
        
        return call;
    }
    
    void MethodCall::parseArguments(chime::parser& parser, MethodCall* call)
    {
        chime::token* t;
        Node*         node;
        
        parser.nextTokenValue("(");
        
        while (true)
        {
            t = parser.look_ahead();
            
            if (t->empty() || t->equal_to(")"))
            {
                // handle the case of call() do ...
                if (parser.look_ahead(2)->equal_to("do"))
                    call->identifier(call->identifier() + ":");
                
                break;
            }
            
            // handle labels
            if (parser.look_ahead(2)->equal_to(":"))
            {
                call->identifier(call->identifier() + parser.nextTokenValue());
                parser.next_token_value(":");
                if (parser.look_ahead()->equal_to(")"))
                {
                    call->identifier(call->identifier() + ":");
                    // there needs to be a closure after this
                    break;
                }
            }
            
            call->identifier(call->identifier() + ":");
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
            call->addChild(Closure::parse(parser));
        }
    }
    
    std::string MethodCall::nodeName(void) const
    {
        return std::string("Method Call");
    }
    
    std::string MethodCall::stringRepresentation(int depth) const
    {
        std::string                  str;
        std::vector<Node*>::iterator it;
        
        str.append(depth*2, ' ');
        str.append("Method Call: ");
        str.append(this->identifier());
        
        for (it = this->children()->begin(); it != this->children()->end(); ++it)
        {
            str.append("\n");
            str.append((*it)->stringRepresentation(depth+1));
        }
        
        return str;
    }
    
    llvm::Value* MethodCall::codegenWithTarget(chime::code_generator& generator, llvm::Value* target)
    {
        llvm::Value*                 methodNamePtr;
        std::vector<llvm::Value*>    arguments;
        std::vector<Node*>::iterator it;
        
        assert(target);
        
        for (it = this->children()->begin(); it != this->children()->end(); ++it)
        {
            llvm::LoadInst* objectLoad;
            llvm::Value*    argumentValue;
            
            argumentValue = (*it)->codegen(generator);
            assert(argumentValue && "Value of method argument must not be null");
            
            objectLoad = generator.builder()->CreateLoad(argumentValue, "loaded argument");
            
            arguments.push_back(objectLoad);
        }
        
        methodNamePtr = generator.getConstantString(this->identifier());
        
        return generator.getRuntime()->callChimeObjectInvoke(target, methodNamePtr, arguments);
    }
}
