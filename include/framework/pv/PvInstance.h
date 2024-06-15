#pragma once
#include "PvCommon.h"
#include "PvResource.h"
#include "VkBootstrap.h"
#include <sys/stat.h>
#include <vector>
#include <volk.h>


namespace Pyra {

class PvInstance;
struct PvInstanceCreateInfo;

template <> struct CreateInfo_T<PvInstance> {
  using type = PvInstanceCreateInfo;
};
struct PvInstanceCreateInfo : PvInfo<VkInstanceCreateInfo, PvInstance> {
  VkInstanceCreateFlags flags;
  VkApplicationInfo appInfo;
  std::vector<const char *> enabledLayers;
  std::vector<const char *> enabledExtensions;

  friend PvInstance;
  friend PvInfo<VkInstanceCreateInfo, PvInstance>;

private:
  void assign();
};

class PvInstance : public PvResource<VkInstance, PFN_vkDestroyInstance> {

public:
  bool init(PvInstanceCreateInfo &info);

  template <typename... T> PvInstance(PvInstanceCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }

  PvInstance(PvTable*);
};

} // namespace Pyra