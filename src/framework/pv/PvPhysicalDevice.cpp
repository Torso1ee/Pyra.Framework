#include "PV/PvPhysicalDevice.h"
#include "pv/PvCommon.h"
#include "pv/PvBootstrap.h"

namespace Pyra {

PvPhysicalDevice::PvPhysicalDevice(PvTable* t) :table(t), handle(t->physicalDevice) {}
} // namespace Pyra