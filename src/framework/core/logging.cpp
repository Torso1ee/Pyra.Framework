#include "core/logging.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <cstdlib>
#include <cwchar>
namespace Pyra {

std::string wcharToString(const wchar_t *content) {
  auto wLen = std::wcslen(content);
  auto sLen = std::wcstombs(nullptr, content, 0);
  char *pStr = new char[sLen + 1];
  std::wcstombs(pStr, content, sLen);
  pStr[sLen] = '\0';
  return std::string(pStr);
}

std::shared_ptr<spdlog::logger> Logger::logger = nullptr;

std::shared_ptr<spdlog::logger> Logger::get() {
  if (logger == nullptr) {
    logger = spdlog::stdout_color_mt("Pyra");
    logger->set_level(spdlog::level::trace);
  }
  return logger;
}


} // namespace Pyra