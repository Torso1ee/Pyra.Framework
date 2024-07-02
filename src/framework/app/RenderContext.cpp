#include "app/RenderContext.h"
#include "app/VulkanApplication.h"
#include "pv/PvBootstrap.h"
#include "pv/PvFence.h"
#include "pv/PvSemaphore.h"
#include "pv/PvSwapchain.h"
#include <cstdint>

namespace Pyra {

void RenderContext::createSyncObjects() {
  availableSemaphores.resize(2);
  finishedSemaphores.resize(2);
  inFlightFences.resize(2);
  imageInFlight.resize(2);
  for (uint32_t i = 0; i < 2; i++) {
    availableSemaphores[i] = bootstrap->make<PvSemaphore>({});
    finishedSemaphores[i] = bootstrap->make<PvSemaphore>({});
    imageInFlight[i] =
        bootstrap->make<PvFence>({.flags = VK_FENCE_CREATE_SIGNALED_BIT});
  }
}

void RenderContext::init(PvBootstrap *boot) {
  bootstrap = boot;
  createSyncObjects();
}

bool RenderContext::nextFrame() {
  inFlightFences[activeFrame]->wait();
  return false;
}

} // namespace Pyra