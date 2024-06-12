#pragma once
#include <spdlog/logger.h>
#include <string>

namespace Pyra {

std::string wcharToString(const wchar_t *content);

template <typename ptr> std::string printPointer(ptr p) {
  std::ostringstream oss;
  oss << p;
  auto addressStr = oss.str();
  return addressStr;
}

class Logger {

public:
  std::shared_ptr<spdlog::logger> static get();

private:
  static std::shared_ptr<spdlog::logger> logger;
};

#define INFO(...) Logger::get()->info(__VA_ARGS__);
#define WARN(...) Logger::get()->warn(__VA_ARGS__);
#define ERROR(...)                                                             \
  Logger::get()->error("[{}:{}]  {}", wcharToString(__FILEW__), __LINE__,      \
                       __VA_ARGS__);
#define DEBUG(...) Logger::get()->debug(__VA_ARGS__);

} // namespace Pyra