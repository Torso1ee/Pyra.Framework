#include "pv/PvPipelineLayout.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>

namespace Pyra {

void PvPipelineLayoutCreateInfo::assign() {
  info = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      .flags = flags,
      .setLayoutCount = (uint32_t)setLayouts.size(),
      .pSetLayouts = NULLPTR_IF_EMPTY(setLayouts),
      .pushConstantRangeCount = (uint32_t)pushConstantRanges.size(),
      .pPushConstantRanges = NULLPTR_IF_EMPTY(pushConstantRanges),
  };
}

bool PvPipelineLayout::init(PvPipelineLayoutCreateInfo &info) {
  if (info.table->disp.createPipelineLayout(&info.info, info.callback,
                                            &handle) != VK_SUCCESS) {
    ERROR("Failed to create vkpipelineLayout!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = info.table->disp.fp_vkDestroyPipelineLayout;
  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation);
  return true;
}

} // namespace Pyra