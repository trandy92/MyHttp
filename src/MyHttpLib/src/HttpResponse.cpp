#include "HttpResponse.h"

#include <format>
#include <utility>
#include <sstream>

#include "HttpTypes.h"
#include <stdexcept>

namespace MyHttp
{

  HttpResponse::Builder::Builder(ProtocolVersion version,
                                 StatusCode statusCode,
                                 ReasonStr reasonStr,
                                 ContentType contentType)
  {
    mResponse.mVersion = std::move(version);
    mResponse.mStatusCode = std::move(statusCode);
    mResponse.mReasonStr = std::move(reasonStr);
    mResponse.mType = std::move(contentType);
  }

  HttpResponse::Builder& HttpResponse::Builder::Content(::MyHttp::Content content)
  {
    mResponse.mContent = std::move(content);
    return *this;
  }
  HttpResponse::Builder& HttpResponse::Builder::Server()
  {
    mResponse.mServerString = ServerString;
    return *this;
  }

  HttpResponse HttpResponse::Builder::Build()
  {
    return std::move(mResponse);
  }

  std::string GetVersionString(const ProtocolVersion& version)
  {
    static_assert(static_cast<int>(ProtocolVersion::kNoEntries) == 1);
    switch (version)
    {
    case ProtocolVersion::Version_1_1:
      return "Http/1.1";
    default:
      throw std::invalid_argument("Version not yet considered");
    }
  }

  std::string GetStatusCodeString(const StatusCode& statusCode)
  {
    static_assert(static_cast<int>(StatusCode::kNoEntries) == 2);
    switch (statusCode)
    {
    case StatusCode::c_200:
      return "200";
    case StatusCode::c_404:
      return "404";
    default:
      throw std::invalid_argument("Status code not yet considered");
    }
  }

  std::string GetContentTypeString(const ContentType& contentType)
  {
    static_assert(static_cast<int>(ContentType::kNoEntries) == 5);
    switch (contentType)
    {
    case ContentType::text_html:
      return "text/html";
    case ContentType::image_jpeg:
      return "image/jpeg";
    case ContentType::image_png:
      return "image/png";
    case ContentType::application_json:
      return "application/json";
    case ContentType::application_octet_stream:
      return "application/octet_stream";
    default:
      return "application/octet_stream";
    }
  }

  std::string HttpResponse::toString()
  {
    std::stringstream response{};
    response << GetVersionString(mVersion) << " " << GetStatusCodeString(mStatusCode) << " " << mReasonStr << "\n"
             << "Content-Type: " << GetContentTypeString(mType) << "\n";

    if (mServerString.has_value())
    {
      response << "Server: " << *mServerString << "\n";
    }
    response << "Content-Length: ";
    if (mContent.has_value())
    {
      response << mContent->length() << "\n\n" << mContent.value();
    }
    else
    {
      response << "0";
    }

    return response.str();
  }

  Content GetNotFoundHtml(std::string path)
  {
    return std::format(R"(<!DOCTYPE html>
<html>
<head><title>404 Not Found</title></head>
<body>
<h1>Not Found</h1>
<p>The requested URL {} was not found on this server.</p>
</body>
</html>)",
                       path);
  }
} // namespace MyHttp
