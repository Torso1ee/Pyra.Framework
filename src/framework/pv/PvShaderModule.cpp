#include "pv/PvShaderModule.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include <volk.h>

namespace Pyra {

void PvShaderModuleCreateInfo::assign() {
  info = {.flags = flags,
          .codeSize = codes.size(),
          .pCode = NULLPTR_IF_EMPTY(codes)};
}

bool PvShaderModule::init(PvShaderModuleCreateInfo &info) {
  if (info.table->disp.createShaderModule(&info.info, info.callback, &handle) !=
      VK_SUCCESS) {
    ERROR("Failed to create vkShaderModule!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = info.table->disp.fp_vkDestroyShaderModule;
  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation);
  return true;
}

} // namespace Pyra