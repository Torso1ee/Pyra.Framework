#include "app/VulkanApplication.h"
#include "core/logging.h"
#include "pv/PvCommon.h"
#include "pv/PvFramebuffer.h"
#include "pv/PvImage.h"
#include <cstdint>
#include <memory>
#include <stdexcept>
#define REPORT_COMPONENT(component)                                            \
  if (bootstrap.init.component != nullptr)                                     \
    return bootstrap.init.component;                                           \
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
      .withInstanceBuilder([](auto builder) {
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
      .withPhysicalDeviceSelector([](auto builder) {

      })
      .withDeviceBuilder([](auto builder) {})
      .withWindow()
      .withSurface()
      .withSwapchainBuilder([](auto builder) {

      })
      .build();
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
  if (bootstrap.table.inst_disp.is_populated())
    return bootstrap.table.inst_disp;
  else {
    auto message = "instance function set up failed!";
    ERROR(message)
    throw std::runtime_error(message);
  }
}

vkb::DispatchTable VulkanApplication::vkd() {
  if (bootstrap.table.disp.is_populated())
    return bootstrap.table.disp;
  else {
    auto message = "device function set up failed!";
    ERROR(message)
    throw std::runtime_error(message);
  }
}

void VulkanApplication::registerEvent() {}

void VulkanApplication::preRun() {
  setUpBootstrap();
  archiveSwapchainData();
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
  bootstrap.createSwapchain();
  archiveSwapchainData();
}

void VulkanApplication::archiveSwapchainData() {
  activeIndex = 0;
  auto images = bootstrap.table.swapchain.get_images().value();
  auto imageViews = bootstrap.table.swapchain.get_image_views().value();
  swapchainDatas.resize(images.size());
  for (uint32_t i = 0; i < images.size(); i++) {
    swapchainDatas[i].image =
        std::make_shared<PvImage>(&bootstrap.table, images[i]);
    swapchainDatas[i].imageView =
        std::make_shared<PvImageView>(&bootstrap.table, imageViews[i]);
    createFramebuffer(swapchainDatas[i]);
  }
}

void VulkanApplication::createFramebuffer(SwapchainData &data) {

  VkFramebufferCreateFlags flags;
  VkRenderPass renderPass;
  std::vector<VkImageView> attachments;
  uint32_t width;
  uint32_t height;
  uint32_t layers;

  CreateInfo<PvFramebuffer> info{
      .attachments = {data.imageView->handle},
      .width = bootstrap.table.swapchain.extent.width,
      .height = bootstrap.table.swapchain.extent.height,
      .layers = 1};
  data.framebuffer = bootstrap.make<PvFramebuffer>(info);
}

} // namespace Pyra