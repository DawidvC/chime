// chime: LocalScopedNode.h

#ifndef LOCAL_SCOPED_NODE_H
#define LOCAL_SCOPED_NODE_H

#include "ScopedNode.h"

namespace chime
{
    class LocalScopedNode : public ast::ScopedNode
    {
    public:
        virtual std::string    getIdentifier() const;
        virtual ast::Variable* createVariable(const std::string& identifier);
    };
}

#endif // LOCAL_SCOPED_NODE_H
