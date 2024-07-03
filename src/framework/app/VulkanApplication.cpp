#include "app/VulkanApplication.h"
#include "core/logging.h"
#include "pv/PvQueue.h"
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

#define REPORT_COMPONENT(component)                                            \
  if (bootstrap->init.component != nullptr)                                    \
    return bootstrap->init.component;                                          \
  else {                                                                       \
    auto message = #component " set up failed!";                               \
    ERROR(message)                                                             \
    throw std::runtime_error(message);                                         \
  }

namespace Pyra {

static void framebufferResizeCallback(void *data, ResizedEventArgs args) {
  auto app = reinterpret_cast<VulkanApplication *>(data);
  app->framebufferResized = true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL
customDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                    void *pUserData) {
  INFO("validation layer: {}", pCallbackData->pMessage);
  return VK_TRUE;
}

void VulkanApplication::configure() {}

void VulkanApplication::setUpBootstrap() {
  bootstrap
      ->withInstanceBuilder([](auto builder) {
        builder.set_debug_callback(customDebugCallback)
            .set_debug_messenger_severity(
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
            .set_debug_messenger_type(
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
            .request_validation_layers();
      })
      ->withPhysicalDeviceSelector([](auto builder) {

      })
      ->withDeviceBuilder([](auto builder) {})
      ->withWindow()
      ->withSurface()
      ->withSwapchainBuilder([](auto builder) {

      })
      ->build();
}

std::shared_ptr<PvInstance> VulkanApplication::instance(){
    REPORT_COMPONENT(instance)}

std::shared_ptr<WindowBase> VulkanApplication::window(){
    REPORT_COMPONENT(window)}

std::shared_ptr<PvPhysicalDevice> VulkanApplication::physicalDevice(){
    REPORT_COMPONENT(physicalDevice)}

std::shared_ptr<PvDevice> VulkanApplication::device(){REPORT_COMPONENT(device)}

std::shared_ptr<PvSurface> VulkanApplication::surface(){
    REPORT_COMPONENT(surface)}

std::shared_ptr<PvSwapchain> VulkanApplication::swapchain() {
  REPORT_COMPONENT(swapchain)
}

void VulkanApplication::registerEvent() {
  window()->resized += {static_cast<void *>(this), framebufferResizeCallback};
}

void VulkanApplication::preRun() {
  setUpBootstrap();
  getQueue();

  createRenderPass();
  createPipeline();
  createFramebuffers();
  createCommandPool();
  createCommandBuffers();
  createSyncObjects();
  registerEvent();
}

bool VulkanApplication::perFrame() {
  inFlightFences[activeFrame]->wait();
  uint32_t imageIndex;
  auto result = bootstrap->init.swapchain->acquireNextImageKHR(
      &imageIndex, availableSemaphores[activeFrame]->handle);
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    return false;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swapchain image. Error ");
  }
  recordCommandBuffer(imageIndex);
  inFlightFences[activeFrame]->reset();
  SubmitInfo submitInfo{
      .submitInfos =
          {{.waitSemaphores = {availableSemaphores[activeFrame]->handle},
            .waitDstStageMask = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT},
            .commandBuffers = {commandBuffers->get(activeFrame).handle},
            .signalSemaphores = {finishedSemaphores[activeFrame]->handle}}},
      .fence = inFlightFences[activeFrame]->handle};
  if (queues.graphics->submit(submitInfo) != VK_SUCCESS) {
    throw std::runtime_error("failed to submit draw command buffer");
  }
  PvPresentInfo presentInfo{
      .waitSemaphores = {finishedSemaphores[activeFrame]->handle},
      .swapchains = {swapchain()->handle},
      .imageIndices = {imageIndex},
  };
  result = queues.present->present(presentInfo);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    return false;
  else if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swapchain image. Error");
  }
  activeFrame = (activeFrame + 1) % setting.maxFramesInFlight;
  return true;
}

void VulkanApplication::internalRun() {
  if (device() != nullptr && window() != nullptr) {
    while (!window()->shouldClose()) {
      window()->pollEvent();
      if (!perFrame() || framebufferResized)
        recreateSwapchain();
    }
    device()->WaitIdle();
  }
}

void VulkanApplication::recreateSwapchain() {
  auto extent = window()->getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = window()->getExtent();
    window()->waitEvent();
  }
  device()->WaitIdle();
  bootstrap->createSwapchain();
  createFramebuffers();
  framebufferResized = false;
}

void VulkanApplication::getQueue() {
  queues.graphics = bootstrap->getQueue(vkb::QueueType::graphics),
  queues.present = bootstrap->getQueue(vkb::QueueType::present);
}

void VulkanApplication::createCommandPool() {
  PvCommandPoolCreateInfo info{
      .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
      .queueFamilyIndex = queues.present->index};
  commandPool = bootstrap->make<PvCommandPool>(info);
}

void VulkanApplication::createCommandBuffers() {
  PvCommandBuffersAllocateInfo info{.commandPool = commandPool->handle,
                                    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                                    .commandBufferCount =
                                        setting.maxFramesInFlight};
  commandBuffers = bootstrap->make<PvCommandBuffers>(info);
}

void VulkanApplication::createSyncObjects() {
  availableSemaphores.resize(setting.maxFramesInFlight);
  finishedSemaphores.resize(setting.maxFramesInFlight);
  inFlightFences.resize(setting.maxFramesInFlight);
  for (uint32_t i = 0; i < setting.maxFramesInFlight; i++) {
    availableSemaphores[i] = bootstrap->make<PvSemaphore>({});
    finishedSemaphores[i] = bootstrap->make<PvSemaphore>({});
    inFlightFences[i] =
        bootstrap->make<PvFence>({.flags = VK_FENCE_CREATE_SIGNALED_BIT});
  }
}

} // namespace Pyra