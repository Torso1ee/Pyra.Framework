#pragma once
#include <tuple>

namespace Pyra {
template <typename Func> struct function_traits {
  using return_type = void;
};

template <typename Ret, typename... Args>
struct function_traits<Ret (*)(Args...)> {
  using return_type = Ret;
  using arg_types = std::tuple<Args...>;
};

} // namespace Pyra