#pragma once

#include <string>
#include "ResourceManager.h"
#include "TcpServer.h"
#include "Utils.h"

namespace MyHttp
{
  class HttpServer
  {
  public:
    HttpServer();
    void Write(const std::string& msg);

  private:
    TcpServer mTcpServer;
    ResourceManager mResourceManager{mFs, ResourceConfig{.resourcesDir = "D:/sandbox/myFolder"}};
    MyHttpFilesystem mFs;
  };
} // namespace MyHttp
