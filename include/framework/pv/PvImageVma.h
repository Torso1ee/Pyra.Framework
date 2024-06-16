
#define NOGDI
#include "pv/PvResource.h"
#include "vk_mem_alloc.h"
#include <cstdint>
#include <vector>
#include <volk.h>

namespace Pyra {

struct VkImageVma {
  VkImage image;
  VmaAllocation meomry;
};

class PvImageVma;
struct PvImageVmaCreateInfo;

template <> struct CreateInfo_T<PvImageVma> {
  using type = PvImageVmaCreateInfo;
};

struct PvImageVmaCreateInfo : PvInfo<VkImageCreateInfo, PvImageVma> {

  VkImageCreateFlags flags;
  VkImageType imageType;
  VkFormat format;
  VkExtent3D extent;
  uint32_t mipLevels;
  uint32_t arrayLayers;
  VkSampleCountFlagBits samples;
  VkImageTiling tiling;
  VkImageUsageFlags usage;
  VkSharingMode sharingMode;
  std::vector<uint32_t> queueFamilyIndices;
  VkImageLayout initialLayout;
  VmaAllocationCreateInfo allocInfo;

  friend PvImageVma;
  friend PvInfo<VkImageCreateInfo, PvImageVma>;

private:
  void assign();
};

using PFN_vmaDestroyImage =
    void (*)(VmaAllocator VMA_NOT_NULL allocator,
             VkImage VMA_NULLABLE_NON_DISPATCHABLE image,
             VmaAllocation VMA_NULLABLE allocation);

class PvImageVma : public PvResource<VkImageVma, PFN_vmaDestroyImage> {

  bool init(PvImageVmaCreateInfo &info);

  template <typename... T> PvImageVma(PvImageVmaCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }
};

} // namespace Pyra