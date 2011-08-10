// chime: method_definition.cpp

#include "method_definition.h"
#include "compiler/parser/parser.h"
#include "compiler/codegen/code_generator.h"

namespace ast
{
    method_definition::method_definition(chime::parser& parser)
    {
        // "method identifier"
        parser.next_token_value("method");
        
        this->setIdentifier(parser.next_token_value());
        this->setParameters(ParameterSetRef(new ParameterSet(parser)));
        _bodyBlock = CodeBlock::nextBlock(parser);
    }
    
    std::string method_definition::nodeName(void) const
    {
        return std::string("method definition");
    }
    std::string method_definition::stringRepresentation(int depth) const
    {
        std::string s;
        
        s.append(depth*2, ' ');
        s.append("method: ");
        s.append(this->getIdentifier());
        
        return s;
    }
    
    NodeRef method_definition::getBody(void) const
    {
        return _bodyBlock;
    }
    
    llvm::Value* method_definition::codegen(chime::code_generator& generator)
    {
        return this->createMethod(generator, this->getIdentifier(), this->getBody(), this->getParameters()->child_count());
    }
}
