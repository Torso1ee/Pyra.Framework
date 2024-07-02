#pragma once
#include "pv/PvDebugUtilsMessenger.h"
#include "core/Core.h"
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
  table = info.table;
  VkResult result = table->inst_disp.createDebugUtilsMessengerEXT(
      &info.info, info.callback, &handle);
  if (result != VK_SUCCESS) {
    ERROR("Failed to create vkDebugUtilsMessengerEXT!");
    return false;
  }

  if (!setDctor)
    setDeconstructor(table->inst_disp.fp_vkDestroyDebugUtilsMessengerEXT);

  manage(handle, std::make_tuple(table->instance, handle, info.callback),
         info.operation, {info.table->instance.instance});
  return true;
}

PvDebugUtilsMessenger::PvDebugUtilsMessenger(PvTable *t, ManageOperation op) {
  table = t;
  handle = t->instance.debug_messenger;

  if (!setDctor)
    setDeconstructor(table->inst_disp.fp_vkDestroyDebugUtilsMessengerEXT);

  manage(handle,
         std::make_tuple(t->instance, handle, t->instance.allocation_callbacks),
         op, {t->instance});
}

} // namespace Pyra