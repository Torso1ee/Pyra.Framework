#pragma once
#include <initializer_list>
#include <memory>
#include <tuple>
#include <utility>

namespace Pyra {
template <typename Func> struct function_traits {
  using return_type = void;
};

template <typename Ret, typename... Args>
struct function_traits<Ret (*)(Args...)> {
  using return_type = Ret;
  using arg_types = std::tuple<Args...>;
};

template <typename T, typename... Args> struct Nullable {

  std::shared_ptr<T> ptr;

  bool hasValue() { return ptr != nullptr; }

  void operator=(std::initializer_list<T> list) {}
};
} // namespace Pyra