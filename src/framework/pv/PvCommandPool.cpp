#include "pv/PvCommandPool.h"
#include "core/Core.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommandBuffers.h"
#include "pv/PvCommon.h"
#include <algorithm>
#include <cstdint>
#include <memory>
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

  if (!setDctor)
    setDeconstructor(info.table->disp.fp_vkDestroyCommandPool);

  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation, {info.table->device.device});
  return true;
}

PvCommandBuffer
PvCommandPool::requestCommandBuffer(VkCommandBufferLevel level) {
  if (level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
    auto it = std::find_if(primaryCommandBuffers.cbegin(),
                           primaryCommandBuffers.cend(),
                           [](auto b) { return !b->active; });
    if (it != primaryCommandBuffers.cend()) {
      (*it)->active = true;
      return (*it)->get(0);
    } else {
      PvCommandBuffersAllocateInfo info{.commandPool = handle,
                                        .level =
                                            VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                                        .commandBufferCount = 1};
      info.setTable(table);
      auto buffer = std::make_shared<PvCommandBuffers>(info);
      primaryCommandBuffers.push_back(buffer);
      buffer->active = true;
      return buffer->get(0);
    }
  }
  else {
    auto it = std::find_if(secondCommandBuffers.cbegin(),
                           secondCommandBuffers.cend(),
                           [](auto b) { return !b->active; });
    if (it != secondCommandBuffers.cend()) {
      (*it)->active = true;
      return (*it)->get(0);
    } else {
      PvCommandBuffersAllocateInfo info{.commandPool = handle,
                                        .level =
                                            VK_COMMAND_BUFFER_LEVEL_SECONDARY,
                                        .commandBufferCount = 1};
      info.setTable(table);
      auto buffer = std::make_shared<PvCommandBuffers>(info);
      secondCommandBuffers.push_back(buffer);
      buffer->active = true;
      return buffer->get(0);
    }
  }
}

} // namespace Pyra