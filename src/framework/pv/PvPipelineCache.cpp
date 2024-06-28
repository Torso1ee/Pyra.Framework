#include "pv/PvPipelineCache.h"
#include "pv/PvBootstrap.h"
#include "vulkan/vulkan_core.h"

namespace Pyra {

void PvPipelineCacheCreateInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
          .flags = flags,
          .initialDataSize = initialDataSize,
          .pInitialData = pInitialData};
}

bool PvPipelineCache::init(PvPipelineCacheCreateInfo &info) {
  table = info.table;
  if (info.table->disp.createPipelineCache(&info.info, info.callback,
                                           &handle) != VK_SUCCESS) {
    ERROR("Failed to create vkpipelinecache!");
    return false;
  }

  if (!setDctor)
    setDeconstructor(info.table->disp.fp_vkDestroyPipelineCache);

  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation, {info.table->device.device});
  return true;
}

} // namespace Pyra