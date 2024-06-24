#include "pv/PvImage.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <volk.h>

namespace Pyra {

void PvImageCreateInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
          .flags = flags,
          .imageType = imageType,
          .format = format,
          .extent = extent,
          .mipLevels = mipLevels,
          .arrayLayers = arrayLayers,
          .samples = samples,
          .tiling = tiling,
          .usage = usage,
          .sharingMode = sharingMode,
          .queueFamilyIndexCount = (uint32_t)queueFamilyIndices.size(),
          .pQueueFamilyIndices = NULLPTR_IF_EMPTY(queueFamilyIndices),
          .initialLayout = initialLayout};
}

bool PvImage::init(PvImageCreateInfo &info) {
  table = info.table;
  if (info.table->disp.createImage(&info.info, info.callback, &handle) !=
      VK_SUCCESS) {
    ERROR("Failed to create vkImage!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = info.table->disp.fp_vkDestroyImage;
  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation);
  return true;
}

PvImage::PvImage(PvTable *t, VkImage image, ManageOperation op) {
  table = t;
  handle = image;
  if (deconstuctor == nullptr)
    deconstuctor = t->disp.fp_vkDestroyImage;
  manage(handle, std::make_tuple(t->device.device, handle, nullptr), op);
}

} // namespace Pyra