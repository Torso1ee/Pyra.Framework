#pragma once
#define NOGDI
#include "pv/PvResource.h"
#include "vk_mem_alloc.h"
#include <cstdint>
#include <vector>
#include <volk.h>

namespace Pyra {

struct VkBufferVma {
  VkBuffer Buffer;
  VmaAllocation meomry;
};

class PvBufferVma;
struct PvBufferVmaCreateInfo;

template <> struct CreateInfo_T<PvBufferVma> {
  using type = PvBufferVmaCreateInfo;
};

struct PvBufferVmaCreateInfo : PvInfo<VkBufferCreateInfo, PvBufferVma> {

  VkBufferCreateFlags flags;
  VkDeviceSize size;
  VkBufferUsageFlags usage;
  VkSharingMode sharingMode;
  std::vector<uint32_t> queueFamilyIndices;
  VmaAllocationCreateInfo allocInfo;

  friend PvBufferVma;
  friend PvInfo<VkBufferCreateInfo, PvBufferVma>;

private:
  void assign();
};

using PFN_vmaDestroyBuffer =
    void (*)(VmaAllocator VMA_NOT_NULL allocator,
             VkBuffer VMA_NULLABLE_NON_DISPATCHABLE Buffer,
             VmaAllocation VMA_NULLABLE allocation);

class PvBufferVma : public PvResource<VkBufferVma *, PFN_vmaDestroyBuffer> {
public:
  bool init(PvBufferVmaCreateInfo &info);

  template <typename... T>
  PvBufferVma(PvBufferVmaCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }
};

} // namespace Pyra