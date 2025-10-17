#pragma once

#include <optional>
#include "HttpTypes.h"

namespace MyHttp
{
  class HttpResponse
  {
  public:
    class Builder;

    std::optional<StatusCode> mStatusCode;
    ReasonStr mReasonStr;
    ProtocolVersion mVersion;
    ContentType mType;
    ContentLength mContentLength;
    std::string mServerString;
    Content mContent;

    std::string toString();

  private:
    HttpResponse() = default;
  };
  class HttpResponse::Builder
  {
  public:
    Builder() = default;

    Builder& Status(StatusCode statusCode);
    Builder& ReasonString(ReasonStr reasonStr);
    Builder& Version(ProtocolVersion protocolVersion);
    Builder& ContentLength(ContentLength contentLength);
    Builder& Server();
    Builder& Type(ContentType type);
    Builder& Content(Content type);

    HttpResponse Build();

  private:
    HttpResponse mResponse;
  };

} // namespace MyHttp
