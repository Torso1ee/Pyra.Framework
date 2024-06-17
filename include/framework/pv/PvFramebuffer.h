#pragma once
#include "pv/PvResource.h"
#include <vector>
#include <volk.h>
namespace Pyra {

class PvFramebuffer;
struct PvFramebufferCreateInfo;

template <> struct CreateInfo_T<PvFramebuffer> {
  using type = PvFramebufferCreateInfo;
};

struct PvFramebufferCreateInfo
    : PvInfo<VkFramebufferCreateInfo, PvFramebuffer> {

  VkFramebufferCreateFlags flags;
  VkRenderPass renderPass;
  std::vector<VkImageView> attachments;
  uint32_t width;
  uint32_t height;
  uint32_t layers;

  friend PvFramebuffer;
  friend PvInfo<VkFramebufferCreateInfo, PvFramebuffer>;

private:
  void assign();
};

class PvFramebuffer
    : public PvResource<VkFramebuffer, PFN_vkDestroyFramebuffer> {
public:
  bool init(PvFramebufferCreateInfo &info);

  template <typename... T>
  PvFramebuffer(PvFramebufferCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }
};

} // namespace Pyra