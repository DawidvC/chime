// chime: Attribute.h

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "Structural.h"

namespace chime
{
    class Attribute : public Structural
    {
    public:
        Attribute(chime::parser& parser);
        
        virtual std::string nodeName() const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        std::string         getIdentifier() const;
        
        llvm::Value*        codegen(chime::code_generator& generator);
        
    protected:
        std::string _identifier;
    };
}

#endif // PROPERTY_DEFINITON_H
