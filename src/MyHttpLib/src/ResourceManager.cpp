#include "ResourceManager.h"

#include <fstream>

namespace MyHttp
{
  ResourceLoadResult ResourceManager::LoadResource(std::string resource)
  {
    if (!fs::exists(mConfig.resourcesDir))
    {
      return ResourceLoadError{.errorMessage = "Resources dir not found."};
    }
    if (resource == "/")
    {
      fs::path concreteResource{mConfig.resourcesDir / "index.html"};
      if (!fs::exists(concreteResource))
      {
        return ResourceLoadError{.errorMessage = "index.html not found"};
      }
      std::ifstream ifs(concreteResource.string());
      std::stringstream strstream;
      strstream << ifs.rdbuf();
      std::string content = strstream.str();
      ifs.close();
      return Resource{.content = std::move(content), .path = std::move(concreteResource)};
    }
    return ResourceLoadError{.errorMessage = "Only / supported for now"};
  }
} // namespace MyHttp
