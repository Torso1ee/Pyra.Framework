#pragma once
#include "pv/PvCommandBuffers.h"
#include "pv/PvResource.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <memory>
#include <vector>
#include <volk.h>
namespace Pyra {

class PvCommandPool;
struct PvCommandPoolCreateInfo;

template <> struct CreateInfo_T<PvCommandPool> {
  using type = PvCommandPoolCreateInfo;
};

struct PvCommandPoolCreateInfo
    : PvInfo<VkCommandPoolCreateInfo, PvCommandPool> {

  VkCommandPoolCreateFlags flags;
  uint32_t queueFamilyIndex;

  friend PvCommandPool;
  friend PvInfo<VkCommandPoolCreateInfo, PvCommandPool>;

private:
  void assign();
};

class PvCommandPool
    : public PvResource<VkCommandPool, PFN_vkDestroyCommandPool> {
public:
  bool init(PvCommandPoolCreateInfo &info);

  PvCommandBuffer requestCommandBuffer(
      VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  template <typename... T>
  PvCommandPool(PvCommandPoolCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }

private:
  std::vector<std::shared_ptr<PvCommandBuffers>> primaryCommandBuffers;
  std::vector<std::shared_ptr<PvCommandBuffers>> secondCommandBuffers;
};

} // namespace Pyra