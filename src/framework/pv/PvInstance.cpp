#include "pv/PvInstance.h"
#include "VkBootstrap.h"
#include "core/Core.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "pv/PvNode.h"
#include "pv/PvResource.h"

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

PvInstance::PvInstance(PvTable *t, ManageOperation op) {
  table = t;
  handle = table->inst_disp.instance;

  if (!setDctor)
    setDeconstructor(table->inst_disp.fp_vkDestroyInstance);

  manage(handle, std::make_tuple(handle, table->instance.allocation_callbacks),
         op);
}

bool PvInstance::init(PvInstanceCreateInfo &info) {
  table = info.table;
  if (vkCreateInstance(&info.info, info.callback, &handle) != VK_SUCCESS) {
    ERROR("Failed to create vkInstance!");
    return false;
  }
  table = info.table;

  if (!setDctor)
    setDeconstructor(table->inst_disp.fp_vkDestroyInstance);

  manage(handle, std::make_tuple(handle, info.callback), info.operation);
  return true;
}

} // namespace Pyra