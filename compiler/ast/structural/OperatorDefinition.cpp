// chime: OperatorDefinition.h

#include "OperatorDefinition.h"
#include "compiler/ast/CodeBlock.h"

namespace chime
{
    OperatorDefinition* OperatorDefinition::parse(parser& parser)
    {
        OperatorDefinition* definition;
        chime::Token*       token;
        
        definition = new OperatorDefinition();
        
        parser.nextTokenValue("operator");
        
        token = parser.lookAhead();
        if (token->precedence() <= 0 || token->equal_to("."))
        {
            parser.addError("Invalid operator overload");
            return definition;
        }
        
        definition->setIdentifier(parser.nextTokenValue());
        
        parser.pushScope(definition);
        
        definition->setParameters(chime::Parameter::parseList(parser));
        if (definition->getParameters().size() != 1)
        {
            parser.addError("An operator overload should have exactly one argument");
        }
        
        definition->_bodyBlock = CodeBlock::parse(parser);
        
        parser.popScope();
        
        return definition;
    }
    
    std::string OperatorDefinition::nodeName() const
    {
        return "Operator Definition";
    }
}
