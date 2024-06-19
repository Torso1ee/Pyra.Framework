#include "app/RenderContext.h"
#include "app/VulkanApplication.h"
#include <cstdint>

namespace Pyra {

void RenderContext::createFramebuffers(SwapchainData &data) {
  for (uint32_t i = 0; i < data.images.size(); i++) {
    CreateInfo<PvFramebuffer> info{.renderPass = contextData.renderPass->handle,
                                   .attachments = {data.imageViews[i]->handle},
                                   .width = data.swapchain->extent.width,
                                   .height = data.swapchain->extent.height,
                                   .layers = 1};
    contextData.framebuffers[i] = bootstrap->make<PvFramebuffer>(info);
  }
}
} // namespace Pyra