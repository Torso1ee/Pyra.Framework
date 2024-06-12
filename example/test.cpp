#include "pv/PvBootstrap.h"

using namespace Pyra;

VKAPI_ATTR VkBool32 VKAPI_CALL
customDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                    void *pUserData) {
  INFO("validation layer: {}",pCallbackData->pMessage);
  return VK_TRUE;
}

int main(int, char **) {
  PvBootstrap boot;
  boot.withInstanceBuilder([](auto builder) {
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
  return 0;
}