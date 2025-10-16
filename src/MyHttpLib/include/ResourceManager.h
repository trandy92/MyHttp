#pragma once

#include <string>
#include <variant>
#include <filesystem>

namespace MyHttp
{
  namespace fs = std::filesystem;

  struct Resource
  {
    std::string content;
    fs::path path;
  };

  struct ResourceConfig
  {
    std::string resourcesDir;
  };
  struct ResourceLoadError
  {
    std::string errorMessage;
  };
  using ResourceLoadResult = std::variant<Resource, ResourceLoadError>;

  class IResourceManager
  {
  public:
    IResourceManager() = default;
    virtual ~IResourceManager() = default;
    virtual ResourceLoadResult LoadResource(std::string resource) = 0;
  };

  class ResourceManager : public IResourceManager
  {
  public:
    ResourceManager(ResourceConfig config) : mConfig(std::move(config)) {}
    ResourceLoadResult LoadResource(std::string resource) override;

  private:
    ResourceConfig mConfig;
  };
} // namespace MyHttp
