#include <iostream>
#include "HttpServer.h"
#include "HttpRequestFactory.h"
#include "Utils.h"
namespace MyHttp
{

  HttpServer::HttpServer()
  {
    mTcpServer.SubscribeToMessages([](const std::string& msg) { std::cout << "[Incoming] " << msg << std::endl; });
    mTcpServer.SubscribeToMessages(
        [this](const std::string& msg)
        {
          auto request = ParseRequest(msg);
          GenericVisit(
              request,
              [this](HttpRequest request)
              {
                static_assert(static_cast<int>(ProtocolVersion::kNoEntries) == 1);
                switch (request.version)
                {
                case ProtocolVersion::Version_1_1:
                  break;
                default:
                  throw std::invalid_argument("Http Protocol Version is not yet supported.");
                  break;
                }

                static_assert(static_cast<int>(Method::kNoEntries) == 4);
                switch (request.method)
                {
                case Method::Get:
                {
                  mResourceManager.LoadResource(request.resource);
                  // request.resource
                }
                break;
                case Method::Post:
                  break;
                case Method::Put:
                  break;
                case Method::Update:
                  break;
                case Method::kNoEntries:
                  throw std::invalid_argument("This should not happen.");
                }
              },
              [](HttpRequestParseError err) { std::cerr << "Failed to parse Http Request" << std::endl; });
        });
  }
  void HttpServer::Write(const std::string& msg)
  {
    mTcpServer.write(msg);
  }
} // namespace MyHttp
