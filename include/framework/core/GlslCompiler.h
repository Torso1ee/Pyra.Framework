#pragma once
#include "shaderc/shaderc.hpp"
#include <cstdint>
#include <memory>
#include <vector>
#include <shaderc/shaderc.h>

namespace Pyra {

class GlslCompiler {

public:
  static std::shared_ptr<shaderc::Compiler> compiler;
  static bool Compile(const char* source,shaderc_shader_kind kind, std::vector<uint32_t>& spirv);
  static bool init;
  static bool  IsValidSpv(const shaderc::SpvCompilationResult& result) ;

};
}