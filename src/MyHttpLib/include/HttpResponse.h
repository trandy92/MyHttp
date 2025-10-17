#pragma once

#include "HttpTypes.h"

namespace MyHttp
{
  class HttpResponse
  {
  public:
    class Builder;

    StatusCode mStatusCode;
    ProtocolVersion mVersion;
    ContentType mType;
    ContentLength mContentLength;
    std::string mServerString;
  };
  class HttpResponse::Builder
  {
  public:
    Builder() = default;

    Builder& Status(StatusCode statusCode);
    Builder& ReasonString(ReasonStr statusCode);
    Builder& Version(ProtocolVersion protocolVersion);
    Builder& ContentLength(ContentLength contentLength);
    Builder& Server();
    Builder& Type(ContentType type);

    HttpResponse Build();

  private:
    HttpResponse mResponse;
  };

} // namespace MyHttp
