#include "pv/PvSurface.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"

namespace Pyra {

PvSurface::PvSurface(PvSurfaceFromWindowInfo &info) {
  table = info.table;
  if (!info.window->createSurface(table->instance,
                                  &handle)) {
    return;
  }
  if (deconstuctor == nullptr)
    deconstuctor = table->inst_disp.fp_vkDestroySurfaceKHR;
  manage(
      handle,
      std::make_tuple(table->instance, handle, nullptr),
      info.operation);
}

PvSurface::PvSurface(PvTable *t, VkSurfaceKHR surface) {
  table = t;
  handle = surface;
  if (deconstuctor == nullptr)
    deconstuctor = table->inst_disp.fp_vkDestroySurfaceKHR;
  manage(handle,
         std::make_tuple(t->instance, handle, t->instance.allocation_callbacks),
         AUTO_MANAGE);
}

} // namespace Pyra