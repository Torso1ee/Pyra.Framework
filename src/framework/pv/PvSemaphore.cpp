#include "pv/PvSemaphore.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "core/logging.h"
#include <volk.h>

namespace Pyra {

void PvSemaphoreCreateInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, .flags = flags};
}

bool PvSemaphore::init(PvSemaphoreCreateInfo &info) {
  table = info.table;
  if (info.table->disp.createSemaphore(&info.info, info.callback, &handle) !=
      VK_SUCCESS) {
    ERROR("Failed to create vkSemaphore!");
    return false;
  }

  if (!setDctor)
    setDeconstructor(info.table->disp.fp_vkDestroySemaphore);

  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation, {info.table->device.device});
  return true;
}

} // namespace Pyra