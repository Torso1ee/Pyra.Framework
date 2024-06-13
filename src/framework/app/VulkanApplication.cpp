#include "app/VulkanApplication.h"
#include "core/logging.h"
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
      .build();
}

std::shared_ptr<PvInstance> VulkanApplication::instance(){
    REPORT_COMPONENT(instance)}

std::shared_ptr<WindowBase> VulkanApplication::window(){
    REPORT_COMPONENT(window)}

std::shared_ptr<PvPhysicalDevice> VulkanApplication::physicalDevice(){
    REPORT_COMPONENT(physicalDevice)}

std::shared_ptr<PvDevice> VulkanApplication::device(){
    REPORT_COMPONENT(device)}

std::shared_ptr<PvSurface> VulkanApplication::surface(){
    REPORT_COMPONENT(surface)}

vkb::InstanceDispatchTable &VulkanApplication::vk() {
  if (bootstrap.init.inst_disp.is_populated())
    return bootstrap.init.inst_disp;
  else {
    auto message = "instance function set up failed!";
    ERROR(message)
    throw std::runtime_error(message);
  }
}

vkb::DispatchTable VulkanApplication::vkd() {

  if (bootstrap.init.disp.is_populated())
    return bootstrap.init.disp;
  else {
    auto message = "device function set up failed!";
    ERROR(message)
    throw std::runtime_error(message);
  }
}
} // namespace Pyra