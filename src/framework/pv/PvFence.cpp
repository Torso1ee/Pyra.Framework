#include "pv/PvFence.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <volk.h>

namespace Pyra {

void PvFenceCreateInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .flags = flags};
}

bool PvFence::init(PvFenceCreateInfo &info) {
  table = info.table;
  if (info.table->disp.createFence(&info.info, info.callback, &handle) !=
      VK_SUCCESS) {
    ERROR("Failed to create vkFence!");
    return false;
  }

  if (!setDctor)
    setDeconstructor(info.table->disp.fp_vkDestroyFence);

  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation, {info.table->device.device});
  return true;
}

VkResult PvFence::wait() {
  return table->disp.waitForFences(1, &handle, VK_TRUE, UINT64_MAX);
}

} // namespace Pyra