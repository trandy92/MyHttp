#include "HttpRequestFactory.h"
#include <optional>
#include <sstream>
#include <istream>
#include <iostream>
#include <unordered_map>
#include <regex>

namespace MyHttp
{
  std::optional<ProtocolVersion> ConvertVersionString(std::string version);

  std::optional<Method> ConvertMethodString(std::string method);

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
    std::regex headerFirstLineRegEx(R"((GET|POST|PUT|UPDATE)\s(\S+)\s(\S+))");
    std::smatch match;
    if (!std::regex_match(line, match, headerFirstLineRegEx))
    {
      return HttpRequestParseError{};
    }
    auto method = ConvertMethodString(match[1]);
    auto version = ConvertVersionString(match[3]);
    if (!method.has_value() || !version.has_value())
    {
      return HttpRequestParseError{};
    }
    HttpRequest req{.method = method.value(), .resource = match[2], .version = version.value()};
    return req;
  }
} // namespace MyHttp
