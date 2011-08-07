#include "Root.h"

namespace ast
{
    Root::Root() :
        node()
    {
    }
    
    Root::~Root()
    {
    }
    
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
}
