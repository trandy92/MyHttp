#pragma once
#include <cstdint>
#include <string>
namespace MyHttp
{
  using ContentLength = uint32_t;
  using ReasonStr = std::string;
  constexpr std::string_view Server = "MyHttp";

  enum class ProtocolVersion
  {
    Version_1_1,
    kNoEntries
  };

  enum class Method
  {
    Get,
    Post,
    Put,
    Update,
    kNoEntries
  };

  enum class StatusCode
  {
    c_200,
  };

  enum class ContentType
  {
    text_html,
  };

} // namespace MyHttp
