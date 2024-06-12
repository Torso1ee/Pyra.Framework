#pragma once
#include <utility>
#include <vector>

namespace Pyra {

// C# style event
template <typename EventArgs> struct Event {

  using CallbackFn = void (*)(void *, EventArgs);

  using Callback = std::pair<void *, CallbackFn>;

  std::vector<Callback> delegates;

  void operator+=(Callback callback) { delegates.push_back(callback); }
  void operator-=(Callback callback) {

    auto it = std::find_if(
        delegates.cbegin(), delegates.cend(), [&callback](auto pair) {
          return pair.first == callback.first && pair.second == callback.second;
        });
    if (it != delegates.cend())
      delegates.erase(it);
  }

  void invoke(EventArgs e) {
    for (auto callback : delegates)
      callback.second(callback.first, e);
  }
};

} // namespace Pyra