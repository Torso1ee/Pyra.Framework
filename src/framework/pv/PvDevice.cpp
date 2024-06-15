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

PvDevice::PvDevice(PvTable *t, ManageOperation op) {
  table = t;
  handle = t->device;
  if (deconstuctor == nullptr)
    deconstuctor = reinterpret_cast<PFN_vkDestroyDevice>(
        table->device.fp_vkGetDeviceProcAddr(handle, "vkDestroyDevice"));
  manage(handle, std::make_tuple(handle, t->device.allocation_callbacks),
         op);
}

void PvDevice::WaitIdle() { table->disp.deviceWaitIdle(); }

bool PvDevice::init(PvDeviceCreateInfo &info) {
  table = info.table;
  if (vkCreateDevice(info.table->physicalDevice.physical_device, &info.info,
                     info.callback, &handle) != VK_SUCCESS) {
    ERROR("Failed to create vkDevice!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = reinterpret_cast<PFN_vkDestroyDevice>(
        table->device.fp_vkGetDeviceProcAddr(handle, "vkDestroyDevice"));
  manage(handle, std::make_tuple(handle, table->device.allocation_callbacks), info.operation);
  return true;
}

} // namespace Pyra