#include "ResourceManager.h"
#include "Utils.h"

namespace MyHttp
{
  ResourceLoadResult ResourceManager::LoadResource(std::string resource)
  {
    if (!mFs.Exists(mConfig.resourcesDir))
    {
      return ResourceLoadError{.errorMessage = "Resources dir not found."};
    }
    if (resource == "/")
    {
      std::string concreteResource{mConfig.resourcesDir + "/index.html"};
      if (!mFs.Exists(concreteResource))
      {
        return ResourceLoadError{.errorMessage = "index.html not found"};
      }
      std::string content = mFs.GetContent(concreteResource);
      return Resource{.content = std::move(content), .path = std::move(concreteResource)};
    }
    return ResourceLoadError{.errorMessage = "Only / supported for now"};
  }
  ResourceManager::ResourceManager(MyHttpFilesystem& fs, ResourceConfig config) : mConfig(std::move(config)), mFs(fs)
  {
  }
} // namespace MyHttp
