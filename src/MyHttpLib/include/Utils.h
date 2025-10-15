#pragma once

#include <variant>

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
