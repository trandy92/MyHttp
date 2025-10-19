#include "ResourceManager.h"
#include "Utils.h"
#include "HttpTypes.h"

namespace MyHttp
{

  ContentType GetContentType(MyHttpFilesystem& fs, std::string path)
  {
    auto ext = fs.GetExtension(path);
    if (ext == ".html")
    {
      return ContentType::text_html;
    }
    if (ext == ".jpg" || ext == ".jpeg")
    {
      return ContentType::image_jpeg;
    }
    if (ext == ".png")
    {
      return ContentType::image_png;
    }
    if (ext == ".json")
    {
      return ContentType::application_json;
    }
    return ContentType::application_octet_stream;
  }

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
        return ResourceLoadError{.errorMessage = "index.html not found."};
      }
      std::string content = mFs.GetContent(concreteResource);
      return Resource{
          .content = std::move(content), .path = std::move(concreteResource), .type = ContentType::text_html};
    }
    else
    {
      std::string concreteResource{mConfig.resourcesDir + resource};
      if (!mFs.Exists(concreteResource))
      {
        return ResourceLoadError{.errorMessage = resource + " not found."};
      }
      std::string content = mFs.GetContent(concreteResource);
      return Resource{.content = std::move(content),
                      .path = std::move(concreteResource),
                      .type = GetContentType(mFs, concreteResource)};
    }
    return ResourceLoadError{.errorMessage = "Only / supported for now."};
  }
  ResourceManager::ResourceManager(MyHttpFilesystem& fs, ResourceConfig config) : mConfig(std::move(config)), mFs(fs)
  {
  }
} // namespace MyHttp
