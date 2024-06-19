#pragma once
#include "pv/PvResource.h"
#include "vulkan/vulkan_core.h"
#include <vector>
#include <volk.h>
namespace Pyra {

class PvBuffer;
struct PvBufferCreateInfo;

template <> struct CreateInfo_T<PvBuffer> {
  using type = PvBufferCreateInfo;
};

struct PvBufferCreateInfo : PvInfo<VkBufferCreateInfo, PvBuffer> {

  VkBufferCreateFlags flags;
  VkDeviceSize size;
  VkBufferUsageFlags usage;
  VkSharingMode sharingMode;
  std::vector<uint32_t> queueFamilyIndices;

  friend PvBuffer;
  friend PvInfo<VkBufferCreateInfo, PvBuffer>;

private:
  void assign();
};

class PvBuffer : public PvResource<VkBuffer, PFN_vkDestroyBuffer> {
public:
  bool init(PvBufferCreateInfo &info);

  template <typename... T> PvBuffer(PvBufferCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }
};

} // namespace Pyra