#include "pv/PvQueue.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <vector>

namespace Pyra {

uint32_t waitSemaphoreCount;
const VkSemaphore *pWaitSemaphores;
const VkPipelineStageFlags *pWaitDstStageMask;
uint32_t commandBufferCount;
const VkCommandBuffer *pCommandBuffers;
uint32_t signalSemaphoreCount;
const VkSemaphore *pSignalSemaphores;

void PvSubmitInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
          .waitSemaphoreCount = (uint32_t)waitSemaphores.size(),
          .pWaitSemaphores = NULLPTR_IF_EMPTY(waitSemaphores),
          .pWaitDstStageMask = NULLPTR_IF_EMPTY(waitDstStageMask),
          .commandBufferCount = (uint32_t)commandBuffers.size(),
          .pCommandBuffers = NULLPTR_IF_EMPTY(commandBuffers),
          .signalSemaphoreCount = (uint32_t)signalSemaphores.size(),
          .pSignalSemaphores = NULLPTR_IF_EMPTY(signalSemaphores)};
}

void PvPresentInfo::assign() {
  info = {
      .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
      .waitSemaphoreCount = (uint32_t)waitSemaphores.size(),
      .pWaitSemaphores = NULLPTR_IF_EMPTY(waitSemaphores),
      .swapchainCount = (uint32_t)swapchains.size(),
      .pSwapchains = NULLPTR_IF_EMPTY(swapchains),
      .pImageIndices = NULLPTR_IF_EMPTY(imageIndices),
  };
}

VkResult PvQueue::submit(SubmitInfo &info) {
  std::vector<VkSubmitInfo> infos;
  for (auto &f : info.submitInfos) {
    f.assign();
    infos.push_back(f.info);
  }
  return table->disp.queueSubmit(handle, info.submitInfos.size(), infos.data(),
                                 info.fence);
}

VkResult PvQueue::present(PvPresentInfo &info) {
  info.assign();
  return table->disp.queuePresentKHR(handle, &info.info);
}

} // namespace Pyra