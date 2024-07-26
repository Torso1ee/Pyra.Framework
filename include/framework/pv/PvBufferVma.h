#pragma once
#include "vulkan/vulkan_core.h"
#define NOGDI
#include "pv/PvResource.h"
#include "vk_mem_alloc.h"
#include <cstdint>
#include <vector>
#include <volk.h>

namespace Pyra {

struct VkBufferVma {
  VkBuffer buffer;
  VmaAllocation memory;
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
  ~PvBufferVma();

  bool init(PvBufferVmaCreateInfo &info);

  size_t update(const uint8_t *data, size_t size, size_t offset = 0);

  uint8_t *map();

  void flush(VkDeviceSize offset = 0, VkDeviceSize size = VK_WHOLE_SIZE);

  void unmap();

  bool mapped() const;

  template <typename... T>
  PvBufferVma(PvBufferVmaCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }

private:
  uint8_t *mappedData = nullptr;
  bool persistent;
  bool coherent;

  void postCreate(VmaAllocationInfo const &allocation_info);

  void clear();
};

} // namespace Pyra