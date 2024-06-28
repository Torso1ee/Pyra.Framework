#include "pv/PvSurface.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"

namespace Pyra {

PvSurface::PvSurface(PvSurfaceFromWindowInfo &info) {
  table = info.table;
  if (!info.window->createSurface(table->instance, &handle)) {
    return;
  }

  if (!setDctor)
    setDeconstructor(info.table->inst_disp.fp_vkDestroySurfaceKHR);

  manage(handle, std::make_tuple(table->instance, handle, nullptr),
         info.operation, {table->instance.instance});
}

PvSurface::PvSurface(PvTable *t, VkSurfaceKHR surface, ManageOperation op) {
  table = t;
  handle = surface;
  if (!setDctor)
    setDeconstructor(t->inst_disp.fp_vkDestroySurfaceKHR);
  manage(handle,
         std::make_tuple(t->instance, handle, t->instance.allocation_callbacks),
         op, {t->instance.instance});
}

} // namespace Pyra