#pragma once
#include "pv/PvCommon.h"
#include "pv/PvResource.h"
#include <vector>
#include <volk.h>

namespace Pyra {

class PvDevice;
struct PvDeviceCreateInfo;

template <> struct CreateInfo_T<PvDevice> {
  using type = PvDeviceCreateInfo;
};

struct PvDeviceCreateInfo : PvInfo<VkDeviceCreateInfo, PvDevice> {
  VkDeviceCreateFlags flags;
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::vector<const char *> enabledLayers;
  std::vector<const char *> enabledExtensions;
  VkPhysicalDeviceFeatures enabledFeatures;

  friend PvDevice;
  friend PvInfo<VkDeviceCreateInfo, PvDevice>;

private:
  void assign();
};

class PvDevice : public PvResource<VkDevice, PFN_vkDestroyDevice> {

public:
  bool init(PvDeviceCreateInfo &info);

  template <typename... T> PvDevice(PvDeviceCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }

  void WaitIdle();

  PvDevice(PvTable *, ManageOperation op = AUTO_MANAGE);
};
} // namespace Pyra