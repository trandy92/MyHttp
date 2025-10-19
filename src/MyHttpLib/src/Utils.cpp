#include "Utils.h"
#include <filesystem>
#include <fstream>

namespace MyHttp
{
  bool MyHttpFilesystem::Exists(std::string_view path) const
  {
    return std::filesystem::exists(path);
  }

  std::string MyHttpFilesystem::GetContent(std::string_view path) const
  {
    std::ifstream ifs(std::filesystem::path(path), std::ios::binary);
    if (!ifs)
    {
      return {};
    }

    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    return content;
  }

  std::string MyHttpFilesystem::GetExtension(std::string_view path) const
  {
    return std::filesystem::path(path).extension().string();
  }
} // namespace MyHttp
