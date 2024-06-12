#pragma once
#include "pv/PvDebugUtilsMessenger.h"
#include "core/logging.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"

namespace Pyra {

VKAPI_ATTR VkBool32 VKAPI_CALL
defaultDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                     VkDebugUtilsMessageTypeFlagsEXT messageType,
                     const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                     void *pUserData) {
  INFO("validation layer: {}", pCallbackData->pMessage);
  return VK_TRUE;
}

void PvDebugUtilsMessengerCreateInfo::assign() {
  if (!pUserData)
    pfnUserCallback = defaultDebugCallback;
  info = {.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
          .flags = flags,
          .messageSeverity = messageSeverity,
          .messageType = messageType,
          .pfnUserCallback = pfnUserCallback,
          .pUserData = pUserData};
}

bool PvDebugUtilsMessenger::init(PvDebugUtilsMessengerCreateInfo &info) {
  VkResult result = info.bootstrap->init.inst_disp.createDebugUtilsMessengerEXT(
      &info.info, info.callback, &handle);
  if (result != VK_SUCCESS) {
    ERROR("Failed to create vkDebugUtilsMessengerEXT!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor =
        info.bootstrap->init.inst_disp.fp_vkDestroyDebugUtilsMessengerEXT;
  manage(handle,
         std::make_tuple(info.bootstrap->init.instance->handle, handle,
                         info.callback),
         info.operation);
  return true;
}

PvDebugUtilsMessenger::PvDebugUtilsMessenger(vkb::Instance &instance) {
  handle = instance.debug_messenger;
  if (deconstuctor == nullptr)
    deconstuctor = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        instance.fp_vkGetInstanceProcAddr(instance.instance,
                                          "vkDestroyDebugUtilsMessengerEXT"));
  manage(handle, std::make_tuple(instance.instance, handle, nullptr),
         AUTO_MANAGE);
}

} // namespace Pyra