#include <iostream>
#include <stdexcept>
#include "HttpServer.h"
#include "HttpRequestFactory.h"
#include "HttpResponse.h"
#include "HttpTypes.h"
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
                  auto response = GenericVisit(
                      resource,
                      [](const Resource& res)
                      {
                        return HttpResponse::Builder(ProtocolVersion::Version_1_1, StatusCode::c_200, "OK", res.type)
                            .Server()
                            .Content(res.content)
                            .Build();
                      },
                      [res = request.resource](const ResourceLoadError& err)
                      {
                        static_assert(static_cast<int>(LoadErrorReason::kNoOfEntries) == 2);
                        switch (err.reason)
                        {
                        case LoadErrorReason::ResourceNotFound:
                          return HttpResponse::Builder(ProtocolVersion::Version_1_1,
                                                       StatusCode::c_404,
                                                       "Not Found",
                                                       ContentType::text_html)
                              .Server()
                              .Content(GetNotFoundHtml(res))
                              .Build();

                        case LoadErrorReason::ConfiguredRootDoesNotExist:
                        case LoadErrorReason::kNoOfEntries:
                          break;
                        }
                        throw std::runtime_error("Error type not yet handled.");
                      });
                  Write(response.toString());
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
