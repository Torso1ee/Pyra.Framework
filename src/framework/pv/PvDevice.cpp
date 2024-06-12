#include "pv/PvDevice.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include <cstdint>

namespace Pyra {

void PvDeviceCreateInfo::assign() {
  info = {
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .flags = flags,
      .queueCreateInfoCount = (uint32_t)queueCreateInfos.size(),
      .pQueueCreateInfos = NULLPTR_IF_EMPTY(queueCreateInfos),
      .enabledLayerCount = (uint32_t)enabledLayers.size(),
      .ppEnabledLayerNames = NULLPTR_IF_EMPTY(enabledLayers),
      .enabledExtensionCount = (uint32_t)enabledExtensions.size(),
      .ppEnabledExtensionNames = NULLPTR_IF_EMPTY(enabledExtensions),
      .pEnabledFeatures = &enabledFeatures,
  };
}

PvDevice::PvDevice(vkb::Device &device) {
  handle = device;
  if (deconstuctor == nullptr)
    deconstuctor = reinterpret_cast<PFN_vkDestroyDevice>(
        device.fp_vkGetDeviceProcAddr(device.device, "vkDestroyDevice"));
  manage(handle, std::make_tuple(handle, device.allocation_callbacks),
         AUTO_MANAGE);
}

bool PvDevice::init(PvDeviceCreateInfo &info) {
  if (vkCreateDevice(info.bootstrap->init.physicalDevice->handle, &info.info,
                     info.callback, &handle) != VK_SUCCESS) {
    ERROR("Failed to create vkDevice!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = vkDestroyDevice;
  manage(handle, std::make_tuple(handle, info.callback), info.operation);
  return true;
}

} // namespace Pyra