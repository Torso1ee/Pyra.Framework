#include "app/VulkanApplication.h"
#include "app/RenderContext.h"
#include "core/logging.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "pv/PvDevice.h"
#include "pv/PvImage.h"
#include "pv/PvImageView.h"
#include <cstdint>
#include <memory>
#include <stdexcept>

#define REPORT_COMPONENT(component)                                            \
  if (bootstrap->init.component != nullptr)                                    \
    return bootstrap->init.component;                                          \
  else {                                                                       \
    auto message = #component " set up failed!";                               \
    ERROR(message)                                                             \
    throw std::runtime_error(message);                                         \
  }

namespace Pyra {

VKAPI_ATTR VkBool32 VKAPI_CALL
customDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                    void *pUserData) {
  INFO("validation layer: {}", pCallbackData->pMessage);
  return VK_TRUE;
}

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

vkb::InstanceDispatchTable &VulkanApplication::vk() {
  if (bootstrap->table.inst_disp.is_populated())
    return bootstrap->table.inst_disp;
  else {
    auto message = "instance function set up failed!";
    ERROR(message)
    throw std::runtime_error(message);
  }
}

vkb::DispatchTable VulkanApplication::vkd() {
  if (bootstrap->table.disp.is_populated())
    return bootstrap->table.disp;
  else {
    auto message = "device function set up failed!";
    ERROR(message)
    throw std::runtime_error(message);
  }
}

void updateSwapchain(void *ptr, SwapchainData *data) {
  auto app = static_cast<VulkanApplication *>(ptr);
  for (auto &context : app->renderContexts) {
    context->swapChainData = data;
  }
}

void VulkanApplication::registerEvent() {
  swapchainUpdated += {static_cast<void *>(this), updateSwapchain};
}

void VulkanApplication::preRun() {
  registerEvent();
  setUpBootstrap();
  archiveSwapchainData();
  for (auto context : renderContexts) {
    context->init();
  }
}

bool VulkanApplication::perFrame() { return true; }

void VulkanApplication::internalRun() {
  if (device() != nullptr && window() != nullptr) {
    while (!window()->shouldClose()) {
      window()->pollEvent();
      perFrame();
    }
    device()->WaitIdle();
  }
}

void VulkanApplication::recreateSwapchain() {
  bootstrap->createSwapchain();
  archiveSwapchainData();
}

void VulkanApplication::getQueue() {
  auto gq = bootstrap->table.device.get_queue(vkb::QueueType::graphics);
  if (!gq.has_value()) {
    ERROR("failed to get graphics queue");
    throw std::runtime_error("failed to get graphics queue");
  } else
    queues.graphics = gq.value();

  auto pq = bootstrap->table.device.get_queue(vkb::QueueType::present);
  if (!pq.has_value()) {
    ERROR("failed to get present queue");
    throw std::runtime_error("failed to get present queue");
  } else
    queues.present = pq.value();
}

void VulkanApplication::archiveSwapchainData() {
  auto images = bootstrap->table.swapchain.get_images().value();
  auto imageViews = bootstrap->table.swapchain.get_image_views().value();
  swapchainData.images.resize(images.size());
  swapchainData.imageViews.resize(imageViews.size());
  swapchainData.currentIndex = 0;
  for (uint32_t i = 0; i < images.size(); i++) {
    swapchainData.images[i] =
        std::make_shared<PvImage>(&bootstrap->table, images[i]);
  }
  for (uint32_t i = 0; i < images.size(); i++) {
    swapchainData.imageViews[i] =
        std::make_shared<PvImageView>(&bootstrap->table, imageViews[i]);
  }
  swapchainData.swapchain = &bootstrap->table.swapchain;
  swapchainUpdated.invoke(&swapchainData);
}

} // namespace Pyra