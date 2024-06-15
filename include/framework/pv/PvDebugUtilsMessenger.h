#pragma once
#include "PvCommon.h"
#include "PvResource.h"
#include <VkBootstrap.h>
#include <volk.h>

namespace Pyra {

class PvDebugUtilsMessenger;
struct PvDebugUtilsMessengerCreateInfo;

template <> struct CreateInfo_T<PvDebugUtilsMessenger> {
  using type = PvDebugUtilsMessengerCreateInfo;
};

VKAPI_ATTR VkBool32 VKAPI_CALL defaultDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

struct PvDebugUtilsMessengerCreateInfo
    : PvInfo<VkDebugUtilsMessengerCreateInfoEXT, PvDebugUtilsMessenger> {

  VkDebugUtilsMessengerCreateFlagsEXT flags;
  VkDebugUtilsMessageSeverityFlagsEXT messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  VkDebugUtilsMessageTypeFlagsEXT messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  PFN_vkDebugUtilsMessengerCallbackEXT pfnUserCallback = nullptr;
  void *pUserData = nullptr;

  friend PvDebugUtilsMessenger;
  friend PvInfo<VkDebugUtilsMessengerCreateInfoEXT, PvDebugUtilsMessenger>;

private:
  void assign();
};

class PvDebugUtilsMessenger
    : public PvResource<VkDebugUtilsMessengerEXT,
                        PFN_vkDestroyDebugUtilsMessengerEXT> {

public:
  bool init(PvDebugUtilsMessengerCreateInfo &info);

  template <typename... T>
  PvDebugUtilsMessenger(PvDebugUtilsMessengerCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }

  PvDebugUtilsMessenger(PvTable*);
};

} // namespace Pyra