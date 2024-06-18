#include "pv/PvPipelineCache.h"
#include "pv/PvBootstrap.h"

namespace Pyra {

void PvPipelineCacheCreateInfo::assign() {
  info = {
      .flags = flags,
  };
}

bool PvPipelineCache::init(PvPipelineCacheCreateInfo &info) {
  if (info.table->disp.createPipelineCache(&info.info, info.callback,
                                           &handle) != VK_SUCCESS) {
    ERROR("Failed to create vkpipelinecache!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = info.table->disp.fp_vkDestroyPipelineCache;
  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation);
  return true;
}

} // namespace Pyra