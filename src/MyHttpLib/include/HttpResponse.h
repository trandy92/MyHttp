#pragma once

#include "HttpTypes.h"

namespace MyHttp
{
  class HttpResponse
  {
  public:
    class Builder;

    StatusCode mStatusCode;
    ReasonStr mReasonStr;
    ProtocolVersion mVersion;
    ContentType mType;
    ContentLength mContentLength;
    std::string mServerString;
    Content mContent;
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
    Builder& Content(Content type);

    HttpResponse Build();

  private:
    HttpResponse mResponse;
  };

} // namespace MyHttp
