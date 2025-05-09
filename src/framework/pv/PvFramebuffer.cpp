#include "pv/PvFramebuffer.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include <cstdint>
#include "core/Core.h"
#include <volk.h>

namespace Pyra {

void PvFramebufferCreateInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
          .flags = flags,
          .renderPass = renderPass,
          .attachmentCount = (uint32_t)attachments.size(),
          .pAttachments = NULLPTR_IF_EMPTY(attachments),
          .width = width,
          .height = height,
          .layers = layers};
}

bool PvFramebuffer::init(PvFramebufferCreateInfo &info) {
  table = info.table;
  if (info.table->disp.createFramebuffer(&info.info, info.callback, &handle) !=
      VK_SUCCESS) {
    ERROR("Failed to create vkFramebuffer!");
    return false;
  }

  if (!setDctor)
    setDeconstructor(info.table->disp.fp_vkDestroyFramebuffer);

  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation, {info.table->device.device});
  return true;
}

} // namespace Pyra