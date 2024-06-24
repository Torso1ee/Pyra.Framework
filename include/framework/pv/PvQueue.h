#include "vulkan/vulkan_core.h"
#include <cstdint>
namespace Pyra {

class PvQueue {
public:
  VkQueue queue;
  uint32_t index = UINT32_MAX;
};
} // namespace Pyra