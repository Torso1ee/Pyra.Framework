#include "pv/PvShaderModule.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "core/Core.h"
#include <volk.h>

namespace Pyra {

void PvShaderModuleCreateInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
          .flags = flags,
          .codeSize = codes.size() * 4,
          .pCode = NULLPTR_IF_EMPTY(codes)};
}

bool PvShaderModule::init(PvShaderModuleCreateInfo &info) {
  table = info.table;
  if (info.table->disp.createShaderModule(&info.info, info.callback, &handle) !=
      VK_SUCCESS) {
    ERROR("Failed to create vkShaderModule!");
    return false;
  }

  if (!setDctor)
    setDeconstructor(info.table->disp.fp_vkDestroyShaderModule);

  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation, {info.table->device.device});
  return true;
}

} // namespace Pyra