#include "pv/PvCommandPool.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include <cstdint>
#include <volk.h>

namespace Pyra {

void PvCommandPoolCreateInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
          .flags = flags,
          .queueFamilyIndex = queueFamilyIndex};
}

bool PvCommandPool::init(PvCommandPoolCreateInfo &info) {
  table = info.table;
  if (info.table->disp.createCommandPool(&info.info, info.callback, &handle) !=
      VK_SUCCESS) {
    ERROR("Failed to create vkCommandPool!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = info.table->disp.fp_vkDestroyCommandPool;
  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation);
  return true;
}

} // namespace Pyra