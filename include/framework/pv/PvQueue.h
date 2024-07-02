#pragma once
#include "pv/PvCommon.h"
#include "pv/PvQueue.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <vector>
#include <volk.h>

namespace Pyra {

class PvQueue;
struct PvSubmitInfo : PvInfo<VkSubmitInfo, PvPlaceHolder> {

  std::vector<VkSemaphore> waitSemaphores;
  std::vector<VkPipelineStageFlags> waitDstStageMask;
  std::vector<VkCommandBuffer> commandBuffers;
  std::vector<VkSemaphore> signalSemaphores;

  friend PvQueue;

private:
  void assign();
};

struct PvPresentInfo : PvInfo<VkPresentInfoKHR, PvPlaceHolder> {

  std::vector<VkSemaphore> waitSemaphores;
  std::vector<VkSwapchainKHR> swapchains;
  std::vector<uint32_t> imageIndices;
  std::vector<VkResult> results;
  friend PvQueue;

private:
  void assign();
};

struct SubmitInfo {
  std::vector<PvSubmitInfo> submitInfos;
  VkFence fence;
};

class PvQueue {
public:
  VkQueue handle;
  uint32_t index = UINT32_MAX;
  PvTable *table;

  PvQueue(PvTable *t, uint32_t index, VkQueue queue)
      : table(t), index(index), handle(queue) {}

  VkResult submit(SubmitInfo &info);

  VkResult present(PvPresentInfo &info);
};
} // namespace Pyra