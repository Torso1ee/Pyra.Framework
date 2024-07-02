#pragma once
#define NOGDI
#include "PvResource.h"
#include "pv/PvCommon.h"
#include <vector>
#include <volk.h>

namespace Pyra {

class PvRenderPass;
struct PvRenderPassCreateInfo;

template <> struct CreateInfo_T<PvRenderPass> {
  using type = PvRenderPassCreateInfo;
};

struct PvSubpassDescription {
  VkSubpassDescriptionFlags flags;
  VkPipelineBindPoint pipelineBindPoint;
  std::vector<VkAttachmentReference> inputAttachments;
  std::vector<VkAttachmentReference> ColorAttachments;
  std::vector<VkAttachmentReference> resolveAttachments;
  std::vector<VkAttachmentReference> depthStencilAttachment;
  std::vector<uint32_t> preserveAttachments;
};

struct PvRenderPassCreateInfo : PvInfo<VkRenderPassCreateInfo, PvRenderPass> {
  VkRenderPassCreateFlags flags;
  std::vector<VkAttachmentDescription> attachments;
  std::vector<PvSubpassDescription> subpasses;
  std::vector<VkSubpassDependency> dependencies;
  std::vector<VkSubpassDescription> spInfos;

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

} // namespace Pyra