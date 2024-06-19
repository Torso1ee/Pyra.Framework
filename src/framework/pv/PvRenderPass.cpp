#include "pv/PvRenderPass.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>

namespace Pyra {

void PvRenderPassCreateInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
          .flags = flags,
          .attachmentCount = (uint32_t)attachments.size(),
          .pAttachments = NULLPTR_IF_EMPTY(attachments),
          .subpassCount = (uint32_t)subpasses.size(),
          .pSubpasses = NULLPTR_IF_EMPTY(subpasses),
          .dependencyCount = (uint32_t)dependencies.size(),
          .pDependencies = NULLPTR_IF_EMPTY(dependencies)};
}

bool PvRenderPass::init(PvRenderPassCreateInfo &info) {
  if (info.table->disp.createRenderPass(&info.info, info.callback, &handle) !=
      VK_SUCCESS) {
    ERROR("Failed to create vkrenderpass!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = info.table->disp.fp_vkDestroyRenderPass;
  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation);
  return true;
}

} // namespace Pyra