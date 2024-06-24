#include "pv/PvImageView.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include <volk.h>

namespace Pyra {

void PvImageViewCreateInfo::assign() {
  
}

bool PvImageView::init(PvImageViewCreateInfo &info) {
  table = info.table;
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