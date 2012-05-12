// chime: try.h

#ifndef TRY_H
#define TRY_H

#include "compiler/ast/control/exception_flow.h"
#include "catch.h"
#include "finally.h"

namespace chime
{
    class Try : public ExceptionFlow
    {
    public:
        Try(chime::parser& parser);
        virtual ~Try();
        
        virtual std::string  nodeName(void) const;
        virtual std::string  stringRepresentation(int depth=0) const;
        
        std::vector<Catch*>* getCatchBlocks(void) const;
        void                 addCatchBlock(Catch* node);
        Finally*             getFinallyBlock(void) const;
        void                 setFinallyBlock(Finally* node);
        
    private:
        Finally*             _finallyBlock;
        std::vector<Catch*>* _catchBlocks;
        
        void parseCatchesAndFinally(chime::parser& parser);
    };
}

#endif // TRY_H
