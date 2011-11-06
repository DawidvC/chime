// chime: method_definition.cpp

#include "method_definition.h"
#include "compiler/parser/parser.h"
#include "compiler/codegen/code_generator.h"
#include "compiler/ast/common/CodeBlock.h"
#include "compiler/ast/variable/LocalVariable.h"
#include "compiler/ast/variable/SharedLocalVariable.h"

namespace ast
{
    method_definition* method_definition::parse(chime::parser& parser)
    {
        method_definition* definition;
        std::string        identifier;
        
        definition = new ast::method_definition();
        
        parser.pushScope(definition);
        
        // "method identifier"
        parser.next_token_value("method");
        
        identifier = parser.nextTokenValue();
        definition->setParameters(chime::ParameterSet::parse(parser));
        
        // create the identifier
        if (definition->getParameters()->length() > 0)
            identifier.append(":");
        
        for (unsigned int i = 1; i < definition->getParameters()->length(); ++i)
        {
            ast::method_parameter* param;
            
            param = definition->getParameters()->parameterAtIndex(i);
            
            identifier.append(param->label());
            identifier.append(":");
        }
        
        definition->setIdentifier(identifier);
        
        definition->defineParametersAsLocalVariables(parser);
        
        definition->_bodyBlock = CodeBlock::parse(parser);
        
        parser.popScope();
        
        return definition;
    }
    
    method_definition::method_definition()
    {
    }
    
    std::string method_definition::nodeName(void) const
    {
        return std::string("method definition");
    }
    std::string method_definition::stringRepresentation(int depth) const
    {
        std::string str;
        
        str.append(depth*2, ' ');
        str.append("method: ");
        str.append(this->getIdentifier());
        
        str.append("\n");
        
        str.append(this->getBody()->stringRepresentation(depth+1));
        
        return str;
    }
    
    NodeRef method_definition::getBody(void) const
    {
        return _bodyBlock;
    }
    
    Variable* method_definition::createVariable(const std::string& identifier)
    {
        // if this variable has been captured, at this point in the state of
        // parsing, then we need to do something special
        if (this->capturedIdentifier(identifier))
        {
            return new SharedLocalVariable(identifier);
        }
        
        return new LocalVariable(identifier);
    }
    
    llvm::Value* method_definition::codegen(chime::code_generator& generator)
    {
        return this->createMethod(generator, this->getIdentifier(), this->getBody(), this->getParameters()->child_count());
    }
}
