#include "pv/PvImageVma.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include <cstdint>
#include <volk.h>

namespace Pyra {

void PvImageVmaCreateInfo::assign() {
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

bool PvImageVma::init(PvImageVmaCreateInfo &info) {
  if (vmaCreateImage(info.table->allocator, &info.info, &info.allocInfo,
                     &handle.image, &handle.meomry, nullptr) != VK_SUCCESS) {
    ERROR("Failed to create vmaImage!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = vmaDestroyImage;
  manage(handle,
         std::make_tuple(info.table->allocator, handle.image, handle.meomry),
         info.operation);
  return true;
}

} // namespace Pyra