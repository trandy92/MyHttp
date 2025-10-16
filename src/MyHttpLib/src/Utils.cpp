#include "Utils.h"
#include <filesystem>
#include <fstream>

namespace MyHttp
{
  bool MyHttpFilesystem::Exists(std::string path) const
  {
    return std::filesystem::exists(path);
  }

  std::string MyHttpFilesystem::GetContent(std::string path) const
  {
    std::ifstream ifs(path);
    std::stringstream strstream;
    strstream << ifs.rdbuf();
    std::string content = strstream.str();
    ifs.close();
    return content;
  }

} // namespace MyHttp
