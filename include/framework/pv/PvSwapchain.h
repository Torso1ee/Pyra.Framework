#pragma once
#include "pv/PvCommon.h"
#include "pv/PvResource.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <memory>
#include <vector>
#include <volk.h>

namespace Pyra {

class PvSwapchain;
class PvImage;
class PvImageView;
struct PvSwapchainCreateInfo;

template <> struct CreateInfo_T<PvSwapchain> {
  using type = PvSwapchainCreateInfo;
};

struct PvSwapchainCreateInfo : PvInfo<VkSwapchainCreateInfoKHR, PvSwapchain> {
  VkSwapchainCreateFlagsKHR flags;
  VkSurfaceKHR surface;
  uint32_t minImageCount;
  VkFormat imageFormat;
  VkColorSpaceKHR imageColorSpace;
  VkExtent2D imageExtent;
  uint32_t imageArrayLayers;
  VkImageUsageFlags imageUsage;
  VkSharingMode imageSharingMode;
  std::vector<uint32_t> queueFamilyIndices;
  VkSurfaceTransformFlagBitsKHR preTransform;
  VkCompositeAlphaFlagBitsKHR compositeAlpha;
  VkPresentModeKHR presentMode;
  VkBool32 clipped;
  VkSwapchainKHR oldSwapchain;

  friend PvSwapchain;
  friend PvInfo<VkSwapchainCreateInfoKHR, PvSwapchain>;

private:
  void assign();
};

class PvSwapchain
    : public PvResource<VkSwapchainKHR, PFN_vkDestroySwapchainKHR> {

public:
  bool init(PvSwapchainCreateInfo &info);

  template <typename... T>
  PvSwapchain(PvSwapchainCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }

  PvSwapchain(PvTable *, ManageOperation op = AUTO_MANAGE);

  void archiveData() override;

  VkResult acquireNextImageKHR(uint32_t* imageIndex,
                               VkSemaphore sema = VK_NULL_HANDLE,
                               VkFence fence = VK_NULL_HANDLE,
                               uint64_t timeout = UINT64_MAX);

  std::vector<std::shared_ptr<PvImage>> images;

  std::vector<std::shared_ptr<PvImageView>> imageViews;

  VkExtent2D extent;
  VkFormat imageFormat;
};
} // namespace Pyra