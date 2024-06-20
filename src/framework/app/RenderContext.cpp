#include "app/RenderContext.h"
#include "app/VulkanApplication.h"
#include <cstdint>

namespace Pyra {

void RenderContextBase::init() {
  createRenderPass();
  createPipeline();
  createFramebuffers();
  createCommandPool();
  createCommandBuffers();
  createSyncObjects();
}

} // namespace Pyra