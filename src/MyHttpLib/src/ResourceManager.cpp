#include "ResourceManager.h"
#include "Utils.h"

namespace MyHttp
{
  ResourceLoadResult ResourceManager::LoadResource(std::string resource)
  {
    MyHttpFilesystem fs;
    if (!fs.Exists(mConfig.resourcesDir))
    {
      return ResourceLoadError{.errorMessage = "Resources dir not found."};
    }
    if (resource == "/")
    {
      std::string concreteResource{mConfig.resourcesDir + "/index.html"};
      if (!fs.Exists(concreteResource))
      {
        return ResourceLoadError{.errorMessage = "index.html not found"};
      }
      std::string content = fs.GetContent(concreteResource);
      return Resource{.content = std::move(content), .path = std::move(concreteResource)};
    }
    return ResourceLoadError{.errorMessage = "Only / supported for now"};
  }
} // namespace MyHttp
