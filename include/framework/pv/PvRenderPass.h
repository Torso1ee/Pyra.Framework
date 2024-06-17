#pragma once
#include "PvResource.h"
#include "pv/PvCommon.h"
#include "vulkan/vulkan_core.h"
#include <vector>
#include <volk.h>
#include <vulkan/vulkan_core.h>

namespace Pyra {

class PvRenderPass;
struct PvRenderPassCreateInfo;

template <> struct CreateInfo_T<PvRenderPass> {
  using type = PvRenderPassCreateInfo;
};

struct PvRenderPassCreateInfo : PvInfo<VkRenderPassCreateInfo, PvRenderPass> {
    VkRenderPassCreateFlags           flags;
    std::vector<VkAttachmentDescription>    attachments;
    std::vector<VkSubpassDescription>    subpasses;
    std::vector<VkSubpassDependency>    dependencies;

  friend PvRenderPass;
  friend PvInfo<VkRenderPassCreateInfo, PvRenderPass>;

private:
  void assign();
};

class PvRenderPass : public PvResource<VkRenderPass, PFN_vkDestroyRenderPass> {
public:
  bool init(PvRenderPassCreateInfo &info);

  template <typename... T>
  PvRenderPass(PvRenderPassCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }
};


}