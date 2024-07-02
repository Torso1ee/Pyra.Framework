#pragma once
#include "pv/PvCommon.h"
#define NOGDI
#include "pv/PvResource.h"
#include <cstdint>
#include <vector>
#include <volk.h>

namespace Pyra {

class PvImage;
struct PvImageCreateInfo;

template <> struct CreateInfo_T<PvImage> {
  using type = PvImageCreateInfo;
};

struct PvImageCreateInfo : PvInfo<VkImageCreateInfo, PvImage> {

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

  friend PvImage;
  friend PvInfo<VkImageCreateInfo, PvImage>;

private:
  void assign();
};

class PvImage : public PvResource<VkImage, PFN_vkDestroyImage> {
public:
  bool init(PvImageCreateInfo &info);

  template <typename... T> PvImage(PvImageCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }

  PvImage(PvTable *t, VkImage image);
};

} // namespace Pyra