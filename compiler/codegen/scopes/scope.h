// scope.h

#ifndef SCOPE_H
#define SCOPE_H

#include <map>
#include <tr1/memory>
#include "llvm/DerivedTypes.h"
#include "llvm/Support/IRBuilder.h"

namespace chime
{
    class Scope
    {
    public:
        Scope();
        virtual ~Scope();
        
        std::string  getName(void) const;
        void         setName(const std::string str);
        
        llvm::Value* getTarget(void) const;
        void         setTarget(llvm::Value* target);
        
        llvm::AllocaInst* getValue(const std::string& name) const;
        void              setValue(const std::string& name, llvm::AllocaInst* value);
    
    protected:
        std::map<std::string, llvm::AllocaInst*>* _values;
        std::map<std::string, llvm::Function*>*   _functions;
        
        llvm::Value* _target;
        std::string  _name;
    };
    
    typedef std::tr1::shared_ptr<chime::Scope> ScopeRef;
}

#endif // SCOPE_H
