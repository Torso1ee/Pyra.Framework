#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include "PvCommon.h"

namespace Pyra {

class PvPhysicalDevice {

public:
  /** @brief Physical device representation */
  VkPhysicalDevice handle;

  PvTable* table;

  PvPhysicalDevice(PvTable* t);

};

} // namespace Pyra