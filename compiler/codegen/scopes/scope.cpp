// scope.cpp

#include "scope.h"

#include <sstream>

namespace chime
{
    Scope::Scope()
    {
        _values    = new std::map<std::string, llvm::AllocaInst*>();
        _functions = new std::map<std::string, llvm::Function*>();
        
        _target    = NULL;
        _name      = "Scope";
        
        _anonymousFunctionCount = 1;
    }
    
    Scope::~Scope()
    {
        if (_values)
            delete _values;
        
        if (_functions)
            delete _functions;
        
        if (_target)
            delete _target;
    }
    
    std::string Scope::getName(void) const
    {
        return _name;
    }
    
    void Scope::setName(const std::string str)
    {
        _name = str;
    }
    
    llvm::Value* Scope::getTarget(void) const
    {
        return _target;
    }
    
    void Scope::setTarget(llvm::Value* target)
    {
        if (_target)
            delete _target;
        
        _target = target;
    }
    
    llvm::AllocaInst* Scope::getValue(const std::string& name) const
    {
        return (*_values)[name];
    }
    
    void Scope::setValue(const std::string& name, llvm::AllocaInst* value)
    {
        (*_values)[name] = value;
    }
    
    std::string Scope::createNewAnonymousFunctionName()
    {
        std::stringstream s;
        
        s << this->getName();
        s << "_anonymous_";
        s << _anonymousFunctionCount;
        
        _anonymousFunctionCount += 1;
        
        return s.str();
    }
}
