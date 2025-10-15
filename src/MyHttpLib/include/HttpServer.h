#pragma once

#include <string>
#include "ResourceManager.h"
#include "TcpServer.h"

namespace MyHttp
{
  class HttpServer
  {
  public:
    HttpServer();
    void Write(const std::string& msg);

  private:
    TcpServer mTcpServer;
    ResourceManager mResourceManager{ResourceConfig{.resourcesDir = "D:/sandbox/myFolder"}};
  };
} // namespace MyHttp
