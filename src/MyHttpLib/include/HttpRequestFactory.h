#pragma once
#include "HttpTypes.h"
#include <variant>
#include <string>

namespace MyHttp
{
  struct HttpRequestParseError
  {
  };

  struct HttpRequest
  {
    Method method;
    std::string resource;
    ProtocolVersion version;
  };

  using HttpRequestParseResult = std::variant<HttpRequest, HttpRequestParseError>;

  HttpRequestParseResult ParseRequest(const std::string& message);
} // namespace MyHttp
