#include "pv/PvSurface.h"
#include "pv/PvCommon.h"
#include "pv/PvBootstrap.h"

namespace Pyra {

PvSurface::PvSurface(PvSurfaceFromWindowInfo &info) {
  if (!info.window->createSurface(info.bootstrap->init.instance->handle,
                                  &handle)) {
    return;
  }
  if (deconstuctor == nullptr)
    deconstuctor = info.bootstrap->init.inst_disp.fp_vkDestroySurfaceKHR;
  manage(
      handle,
      std::make_tuple(info.bootstrap->init.instance->handle, handle, nullptr),
      info.operation);
}

PvSurface::PvSurface(vkb::Instance instance, VkSurfaceKHR surface) {
  handle = surface;
  if (deconstuctor == nullptr)
    deconstuctor = reinterpret_cast<PFN_vkDestroySurfaceKHR>(
        instance.fp_vkGetInstanceProcAddr(instance.instance,
                                          "vkDestroySurfaceKHR"));
  manage(handle, std::make_tuple(instance, handle, instance.allocation_callbacks), AUTO_MANAGE);
}

} // namespace Pyra