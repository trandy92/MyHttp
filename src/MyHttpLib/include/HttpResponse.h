#pragma once

#include <optional>
#include "HttpTypes.h"

namespace MyHttp
{

  Content GetNotFoundHtml(std::string path);

  class HttpResponse
  {
  public:
    class Builder;

    StatusCode mStatusCode;
    ReasonStr mReasonStr;
    ProtocolVersion mVersion;
    ContentType mType;
    std::optional<std::string> mServerString;
    std::optional<Content> mContent;

    std::string toString();

  private:
    HttpResponse() = default;
  };
  class HttpResponse::Builder
  {
  public:
    Builder(ProtocolVersion version, StatusCode statusCode, ReasonStr reasonStr, ContentType contentType);

    Builder& Server();
    Builder& Content(Content content);

    HttpResponse Build();

  private:
    HttpResponse mResponse;
  };

} // namespace MyHttp
