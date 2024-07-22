#include "core/GlslCompiler.h"
#include <spirv/1.2/spirv.hpp>

namespace Pyra {

std::shared_ptr<shaderc::Compiler> GlslCompiler::compiler;
bool GlslCompiler::init = false;

bool GlslCompiler::Compile(const char *source, shaderc_shader_kind kind,
                           std::vector<uint32_t> &spirv) {
  if (!init) {
    compiler = std::make_shared<shaderc::Compiler>();
    init = true;
  }
  auto result = compiler->CompileGlslToSpv(source, kind, "");
  if (IsValidSpv(result)) {
    spirv = std::vector<uint32_t>(result.cbegin(), result.cend());
  }
  return true;
}

bool GlslCompiler::IsValidSpv(const shaderc::SpvCompilationResult &result) {
  if (!result.GetCompilationStatus() == shaderc_compilation_status_success)
    return false;
  size_t length_in_words = result.cend() - result.cbegin();
  if (length_in_words < 5)
    return false;
  const uint32_t *bytes = result.cbegin();
  return bytes[0] == spv::MagicNumber;
}

} // namespace Pyra