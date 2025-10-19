#pragma once
#include <cstdint>
#include <string>
namespace MyHttp
{
  using ContentLength = uint32_t;
  using Content = std::string;
  using ReasonStr = std::string;
  constexpr std::string_view ServerString = "MyHttp";

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
    c_404,
    kNoEntries
  };

  enum class ContentType
  {
    text_html,
    image_jpeg,
    image_png,
    application_json,
    application_octet_stream,
    kNoEntries
  };

} // namespace MyHttp
