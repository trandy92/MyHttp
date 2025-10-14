#pragma once
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

struct HttpRequest
{
  Method method;
  Resource resource;
  ProtocolVersion version;
};

HttpRequest ParseRequest(const std::string& message)
{
  HttpRequest req{.method = Method::Get, .resource = ""};
  return req;
}
