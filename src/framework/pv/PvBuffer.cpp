#include "pv/PvBuffer.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <volk.h>

namespace Pyra {

void PvBufferCreateInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
          .flags = flags,
          .size = size,
          .usage = usage,
          .sharingMode = sharingMode,
          .queueFamilyIndexCount = (uint32_t)queueFamilyIndices.size(),
          .pQueueFamilyIndices = NULLPTR_IF_EMPTY(queueFamilyIndices)};
}

bool PvBuffer::init(PvBufferCreateInfo &info) {
  if (info.table->disp.createBuffer(&info.info, info.callback, &handle) !=
      VK_SUCCESS) {
    ERROR("Failed to create vkBuffer!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = info.table->disp.fp_vkDestroyBuffer;
  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation);
  return true;
}

} // namespace Pyra