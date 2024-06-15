#include "pv/PvSwapchain.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>

namespace Pyra {

void PvSwapchainCreateInfo::assign() {

  info = {.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
          .flags = flags,
          .surface = surface,
          .minImageCount = minImageCount,
          .imageFormat = imageFormat,
          .imageColorSpace = imageColorSpace,
          .imageExtent = imageExtent,
          .imageArrayLayers = imageArrayLayers,
          .imageUsage = imageUsage,
          .imageSharingMode = imageSharingMode,
          .queueFamilyIndexCount = (uint32_t)queueFamilyIndices.size(),
          .pQueueFamilyIndices = queueFamilyIndices.data(),
          .preTransform = preTransform,
          .compositeAlpha = compositeAlpha,
          .presentMode = presentMode,
          .clipped = clipped,
          .oldSwapchain = oldSwapchain};
}

PvSwapchain::PvSwapchain(PvTable *t, ManageOperation op) {
  table = t;
  handle = t->swapchain;
  if (deconstuctor == nullptr)
    deconstuctor = table->disp.fp_vkDestroySwapchainKHR;
  manage(
      handle,
      std::make_tuple(t->device.device, handle, t->device.allocation_callbacks),
      op);
}

bool PvSwapchain::init(PvSwapchainCreateInfo &info) {
  table = info.table;
  if (table->disp.createSwapchainKHR(&info.info, info.callback, &handle) !=
      VK_SUCCESS) {
    ERROR("Failed to create vkSswapchain!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = table->disp.fp_vkDestroySwapchainKHR;
  manage(handle,
         std::make_tuple(table->device.device, handle,
                         table->device.allocation_callbacks),
         info.operation);
  return true;
}

} // namespace Pyra