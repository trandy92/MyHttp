#pragma once
#include <variant>
#include <string>

enum class ProtocolVersion
{
  Version_1_1
};

enum class Method
{
  Get,
  Post,
  Put,
  Update
};

enum class StatusCode
{
  c_200,
};

using Resource = std::string;

struct HttpResponse
{
  ProtocolVersion version;
  StatusCode statusCode;
};

struct HttpRequestParseError
{
};

struct HttpRequest
{
  Method method;
  Resource resource;
  ProtocolVersion version;
};

using HttpRequestParseResult = std::variant<HttpRequest, HttpRequestParseError>;

HttpRequestParseResult ParseRequest(const std::string& message);
