#pragma once
#include "pv/PvResource.h"
#include <volk.h>
namespace Pyra {

class PvSemaphore;
struct PvSemaphoreCreateInfo;

template <> struct CreateInfo_T<PvSemaphore> {
  using type = PvSemaphoreCreateInfo;
};

struct PvSemaphoreCreateInfo : PvInfo<VkSemaphoreCreateInfo, PvSemaphore> {

  VkSemaphoreCreateFlags flags;

  friend PvSemaphore;
  friend PvInfo<VkSemaphoreCreateInfo, PvSemaphore>;

private:
  void assign();
};

class PvSemaphore : public PvResource<VkSemaphore, PFN_vkDestroySemaphore> {
public:
  bool init(PvSemaphoreCreateInfo &info);

  template <typename... T>
  PvSemaphore(PvSemaphoreCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }
};

} // namespace Pyra