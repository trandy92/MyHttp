#pragma once
#include <string>
#include <istream>
#include <iostream>
#include <variant>
#include <regex>
#include <optional>

template<class... Ts>
struct overloaded : Ts...
{
  using Ts::operator()...;
};

template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template<typename Variant, typename... Matchers>
auto GenericVisit(Variant&& v, Matchers&&... m)
{
  return std::visit(overloaded{std::forward<Matchers>(m)...}, std::forward<Variant>(v));
}

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

std::optional<ProtocolVersion> ConvertVersionString(std::string version)
{
  std::unordered_map<std::string, ProtocolVersion> versions = {{"HTTP/1.1", ProtocolVersion::Version_1_1}};
  auto foundVersion = versions.find(version);
  if (foundVersion != versions.end())
  {
    return versions.at(version);
  }
  return std::nullopt;
}

std::optional<Method> ConvertMethodString(std::string method)
{
  std::unordered_map<std::string, Method> methods = {
      {"GET", Method::Get}, {"PUT", Method::Put}, {"POST", Method::Post}, {"UPDATE", Method::Update}};
  auto foundMethod = methods.find(method);
  if (foundMethod != methods.end())
  {
    return methods.at(method);
  }
  return std::nullopt;
}

HttpRequestParseResult ParseRequest(const std::string& message)
{
  std::stringbuf buf(message);
  std::istream istream(&buf);
  std::string line;
  std::getline(istream, line);
  std::getline(istream, line);
  std::regex headerFirstLineRegEx(R"((GET|POST|PUT|UPDATE)\s(\S+)\s(\S+))");
  std::smatch match;
  if (!std::regex_match(line, match, headerFirstLineRegEx))
  {
    HttpRequestParseError err;
    return err;
  }
  auto method = ConvertMethodString(match[1]);
  auto version = ConvertVersionString(match[3]);
  if (!method.has_value() || !version.has_value())
  {
    HttpRequestParseError err;
    return err;
  }
  HttpRequest req{.method = method.value(), .resource = match[2], .version = version.value()};
  return req;
}
