// chime: try.h

#ifndef TRY_H
#define TRY_H

#include "ast/control/exception_flow.h"
#include "ast/control/catch.h"
#include "ast/control/finally.h"

namespace ast
{
    class Try : public ExceptionFlow
    {
    public:
        Try(chime::parser& parser);
        virtual ~Try();
        
        virtual std::string nodeName(void) const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        std::vector<ast::Catch*>* getCatchBlocks(void) const;
        void                      addCatchBlock(ast::Catch* node);
        ast::Finally*             getFinallyBlock(void) const;
        void                      setFinallyBlock(ast::Finally* node);
        
    private:
        ast::Finally*             _finallyBlock;
        std::vector<ast::Catch*>* _catchBlocks;
        
        void parseCatchesAndFinally(chime::parser& parser);
    };
}

#endif // TRY_H
