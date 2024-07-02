#pragma once
#include "pv/PvResource.h"
#include <vector>
#include <volk.h>
namespace Pyra {

class PvFence;
struct PvFenceCreateInfo;

template <> struct CreateInfo_T<PvFence> {
  using type = PvFenceCreateInfo;
};

struct PvFenceCreateInfo : PvInfo<VkFenceCreateInfo, PvFence> {

  VkFenceCreateFlags flags;

  friend PvFence;
  friend PvInfo<VkFenceCreateInfo, PvFence>;

private:
  void assign();
};

class PvFence : public PvResource<VkFence, PFN_vkDestroyFence> {
public:
  bool init(PvFenceCreateInfo &info);

  template <typename... T> PvFence(PvFenceCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }

  VkResult wait();
};

} // namespace Pyra