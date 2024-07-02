#pragma once
#include "pv/PvResource.h"
#include <vector>
#include <volk.h>
namespace Pyra {

class PvPipelineLayout;
struct PvPipelineLayoutCreateInfo;

template <> struct CreateInfo_T<PvPipelineLayout> {
  using type = PvPipelineLayoutCreateInfo;
};

struct PvPipelineLayoutCreateInfo
    : PvInfo<VkPipelineLayoutCreateInfo, PvPipelineLayout> {

  VkPipelineLayoutCreateFlags flags =
      VK_PIPELINE_LAYOUT_CREATE_INDEPENDENT_SETS_BIT_EXT;
  std::vector<VkDescriptorSetLayout> setLayouts;
  std::vector<VkPushConstantRange> pushConstantRanges;

  friend PvPipelineLayout;
  friend PvInfo<VkPipelineLayoutCreateInfo, PvPipelineLayout>;

private:
  void assign();
};

class PvPipelineLayout
    : public PvResource<VkPipelineLayout, PFN_vkDestroyPipelineLayout> {
public:
  bool init(PvPipelineLayoutCreateInfo &info);

  template <typename... T>
  PvPipelineLayout(PvPipelineLayoutCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }
};

} // namespace Pyra