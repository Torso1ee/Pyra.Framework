#pragma once
#include "pv/PvResource.h"
#include <vector>
#include <volk.h>
namespace Pyra {

class PvShaderModule;
struct PvShaderModuleCreateInfo;

template <> struct CreateInfo_T<PvShaderModule> {
  using type = PvShaderModuleCreateInfo;
};

struct PvShaderModuleCreateInfo
    : PvInfo<VkShaderModuleCreateInfo, PvShaderModule> {
  VkShaderModuleCreateFlags flags;
  std::vector<uint32_t> codes;

  friend PvShaderModule;
  friend PvInfo<VkShaderModuleCreateInfo, PvShaderModule>;

private:
  void assign();
};

class PvShaderModule
    : public PvResource<VkShaderModule, PFN_vkDestroyShaderModule> {
public:
  bool init(PvShaderModuleCreateInfo &info);

  template <typename... T>
  PvShaderModule(PvShaderModuleCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }
};

} // namespace Pyra