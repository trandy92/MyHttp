#pragma once

#include "HttpTypes.h"
#include <future>
#include <utility>
#include <variant>
namespace MyHttp
{
  class MyHttpFilesystem
  {
  public:
    virtual bool Exists(std::string_view path) const;
    virtual std::string GetContent(std::string_view path) const;
    virtual std::string GetExtension(std::string_view path) const;
  };

  template<typename F, typename... T>
  auto RunReallyAsync(F&& function, T&&... params)
  {
    return std::async(std::launch::async, std::forward<F>(function), std::forward<T>(params)...);
  }

  // For Generic Visitor pattern
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
} // namespace MyHttp
