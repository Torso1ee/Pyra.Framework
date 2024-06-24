#pragma once
#include "pv/PvResource.h"
#include <vector>
#include <volk.h>
namespace Pyra {

class PvCommandPool;
struct PvCommandPoolCreateInfo;

template <> struct CreateInfo_T<PvCommandPool> {
  using type = PvCommandPoolCreateInfo;
};

struct PvCommandPoolCreateInfo
    : PvInfo<VkCommandPoolCreateInfo, PvCommandPool> {

  VkCommandPoolCreateFlags flags;
  uint32_t queueFamilyIndex;

  friend PvCommandPool;
  friend PvInfo<VkCommandPoolCreateInfo, PvCommandPool>;

private:
  void assign();
};

class PvCommandPool
    : public PvResource<VkCommandPool, PFN_vkDestroyCommandPool> {
public:
  bool init(PvCommandPoolCreateInfo &info);

  template <typename... T>
  PvCommandPool(PvCommandPoolCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }
};

} // namespace Pyra