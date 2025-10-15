#pragma once

namespace MyHttp{

using Resource = std::string;
struct ResourceLoadError
{
    std::string errorMessage;
}
using ResourceLoadResult = std::variant<Resource, ResourceLoadError>;

class IResourceManager:
{
    public:
    virtual IResourceManager = default;
    virtual ResourceLoadResult LoadResource() = 0;
}

class ResourceManager : public IResourceManager
{

}
}