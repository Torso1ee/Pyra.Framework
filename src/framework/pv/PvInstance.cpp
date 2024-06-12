#include "pv/pvInstance.h"
#include "VkBootstrap.h"
#include "core/logging.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"

namespace Pyra {

void PvInstanceCreateInfo::assign() {
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  info = {.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
          .pNext = nullptr,
          .flags = flags,
          .pApplicationInfo = &appInfo,
          .enabledLayerCount = (uint32_t)enabledLayers.size(),
          .ppEnabledLayerNames =
              enabledLayers.size() > 0 ? enabledLayers.data() : nullptr,
          .enabledExtensionCount = (uint32_t)enabledExtensions.size(),
          .ppEnabledExtensionNames = enabledExtensions.size() > 0
                                         ? enabledExtensions.data()
                                         : nullptr};
}

PvInstance::PvInstance(vkb::Instance &instance) {
  handle = instance.instance;
  if (deconstuctor == nullptr)
    deconstuctor = reinterpret_cast<PFN_vkDestroyInstance>(
        instance.fp_vkGetInstanceProcAddr(instance.instance,
                                          "vkDestroyInstance"));
  manage(handle, std::make_tuple(handle, instance.allocation_callbacks),
         AUTO_MANAGE);
}

bool PvInstance::init(PvInstanceCreateInfo &info) {

  if (vkCreateInstance(&info.info, info.callback, &handle) != VK_SUCCESS) {
    ERROR("Failed to create vkInstance!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = vkDestroyInstance;
  manage(handle, std::make_tuple(handle, info.callback), info.operation);
  return true;
}

} // namespace Pyra