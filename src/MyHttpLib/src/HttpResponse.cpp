#include "HttpResponse.h"
#include "HttpTypes.h"

#include <utility>

namespace MyHttp
{
  HttpResponse::Builder& HttpResponse::Builder::Status(StatusCode statusCode)
  {
    mResponse.mStatusCode = std::move(statusCode);
    return *this;
  }
  HttpResponse::Builder& HttpResponse::Builder::Version(ProtocolVersion protocolVersion)
  {
    mResponse.mVersion = std::move(protocolVersion);
    return *this;
  }
  HttpResponse::Builder& HttpResponse::Builder::Type(ContentType type)
  {
    mResponse.mType = std::move(type);
    return *this;
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
  HttpResponse::Builder& HttpResponse::Builder::ReasonString(ReasonStr reason)
  {
    mResponse.mReasonStr = std::move(reason);
    return *this;
  }

  HttpResponse::Builder& HttpResponse::Builder::ContentLength(::MyHttp::ContentLength contentLength)
  {
    mResponse.mContentLength = std::move(contentLength);
    return *this;
  }
  HttpResponse HttpResponse::Builder::Build()
  {
    return std::move(mResponse);
  }
} // namespace MyHttp
