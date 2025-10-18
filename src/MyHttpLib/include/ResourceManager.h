#pragma once

#include "Utils.h"
#include <string>
#include <variant>
#include <filesystem>

namespace MyHttp
{
  class MyHttpFilesystem;
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
  enum class LoadErrorReason
  {
    ResourceNotFound,
    ConfiguredRootDoesNotExist,
    kNoOfEntries
  };
  struct ResourceLoadError
  {
    std::string errorMessage;
    LoadErrorReason reason;
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
    // ResourceManager(ResourceConfig config);
    ResourceManager(MyHttpFilesystem& fs, ResourceConfig config);
    ResourceLoadResult LoadResource(std::string resource) override;

  private:
    ResourceConfig mConfig;
    MyHttpFilesystem& mFs;
  };
} // namespace MyHttp
