#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <volk.h>

namespace Pyra {

VkStructureType sType;
const void *pNext;
VkImageViewCreateFlags flags;
VkImage image;
VkImageViewType viewType;
VkFormat format;
VkComponentMapping components;
VkImageSubresourceRange subresourceRange;

void PvImageViewCreateInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
          .flags = flags,
          .image = image,
          .viewType = viewType,
          .format = format,
          .components = components,
          .subresourceRange = subresourceRange};
}

bool PvImageView::init(PvImageViewCreateInfo &info) {
  if (info.table->disp.createImageView(&info.info, info.callback, &handle) !=
      VK_SUCCESS) {
    ERROR("Failed to create vkImageView!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = info.table->disp.fp_vkDestroyImageView;
  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation);
  return true;
}

PvImageView::PvImageView(PvTable *t, VkImageView v, ManageOperation op) {
  table = t;
  handle = v;
  if (deconstuctor == nullptr)
    deconstuctor = t->disp.fp_vkDestroyImageView;
  manage(handle, std::make_tuple(t->device.device, handle, nullptr), op);
}

} // namespace Pyra