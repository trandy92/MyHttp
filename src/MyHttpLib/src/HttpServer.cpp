#include <iostream>
#include "HttpServer.h"
#include "HttpRequestFactory.h"
#include "HttpResponse.h"
#include "ResourceManager.h"
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
                  auto resource = mResourceManager.LoadResource(request.resource);
                  GenericVisit(
                      resource,
                      [this](const Resource& res)
                      {
                        auto response = HttpResponse::Builder{}
                                            .Server()
                                            .ContentLength(res.content.size())
                                            .Status(StatusCode::c_200)
                                            .ReasonString("OK")
                                            .Version(ProtocolVersion::Version_1_1)
                                            .Type(ContentType::text_html)
                                            .Content(res.content)
                                            .Build();
                        // Write(response);
                      },
                      [](const ResourceLoadError& err)
                      { std::cerr << "Failed with error " << err.errorMessage << std::endl; });
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
    mTcpServer.Write(msg);
  }
} // namespace MyHttp
