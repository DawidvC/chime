#include "Root.h"
#include "compiler/ast/variable/GlobalVariable.h"

namespace ast
{
    std::vector<std::string> Root::getBinaryDependencies(void) const
    {
        return _binaryDependencies;
    }
    void Root::addBinaryDependency(const std::string& dependency)
    {
        _binaryDependencies.push_back(dependency);
    }
    
    std::vector<std::string> Root::getSourceDependencies(void) const
    {
        return _sourceDependencies;
    }
    void Root::addSourceDependency(const std::string& dependency)
    {
        _sourceDependencies.push_back(dependency);
    }
    
    std::string Root::getIdentifier() const
    {
        return std::string("Root");
    }
    
    Variable* Root::createVariable(const std::string& identifier)
    {
        return new GlobalVariable(identifier);
    }
}
