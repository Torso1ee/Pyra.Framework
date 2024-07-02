#include "pv/PvImageView.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include <volk.h>
#include "core/Core.h"
namespace Pyra {

void PvImageViewCreateInfo::assign() {
  info = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .flags = flags,
      .image = image,
      .viewType = viewType,
      .format = format,
      .components = components,
      .subresourceRange = subresourceRange,
  };
}

bool PvImageView::init(PvImageViewCreateInfo &info) {
  table = info.table;
  if (info.table->disp.createImageView(&info.info, info.callback, &handle) !=
      VK_SUCCESS) {
    ERROR("Failed to create vkImageView!");
    return false;
  }

  if (!setDctor)
    setDeconstructor(info.table->disp.fp_vkDestroyImageView);

  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation, {info.image});
  return true;
}

PvImageView::PvImageView(PvTable *t, VkImageView v, ManageOperation op) {
  table = t;
  handle = v;

  if (!setDctor)
    setDeconstructor(t->disp.fp_vkDestroyImageView);

  manage(handle, std::make_tuple(t->device.device, handle, nullptr), op,
         {t->swapchain.swapchain});
}

} // namespace Pyra