#include "pv/PvSwapchain.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "pv/PvImage.h"
#include "pv/PvImageView.h"
#include "core/Core.h"
#include <cstdint>
#include <memory>

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
          .pQueueFamilyIndices = NULLPTR_IF_EMPTY(queueFamilyIndices),
          .preTransform = preTransform,
          .compositeAlpha = compositeAlpha,
          .presentMode = presentMode,
          .clipped = clipped,
          .oldSwapchain = oldSwapchain};
}

PvSwapchain::PvSwapchain(PvTable *t, ManageOperation op) {
  table = t;
  handle = t->swapchain;

  if (!setDctor)
    setDeconstructor(table->disp.fp_vkDestroySwapchainKHR);

  manage(
      handle,
      std::make_tuple(t->device.device, handle, t->device.allocation_callbacks),
      op, {t->device.device});

  archiveData();
}

bool PvSwapchain::init(PvSwapchainCreateInfo &info) {
  table = info.table;
  if (table->disp.createSwapchainKHR(&info.info, info.callback, &handle) !=
      VK_SUCCESS) {
    ERROR("Failed to create vkSswapchain!");
    return false;
  }

  if (!setDctor)
    setDeconstructor(table->disp.fp_vkDestroySwapchainKHR);

  manage(handle,
         std::make_tuple(table->device.device, handle,
                         table->device.allocation_callbacks),
         info.operation, {info.table->device.device});

  archiveData();

  return true;
}

void PvSwapchain::archiveData() {
  if (table->swapchain.swapchain != VK_NULL_HANDLE) {
    extent = table->swapchain.extent;
    imageFormat = table->swapchain.image_format;
    auto imgs = table->swapchain.get_images().value();
    auto views = table->swapchain.get_image_views().value();
    for (uint32_t i = 0; i < imgs.size(); i++) {
      images.push_back(std::make_shared<PvImage>(table, imgs[i]));
      imageViews.push_back(std::make_shared<PvImageView>(table, views[i]));
    }
  } else {
    // TODO
  }
}

} // namespace Pyra