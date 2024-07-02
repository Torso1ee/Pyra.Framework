#pragma once
#define NOGDI
#include "PvResource.h"
#include "pv/PvCommon.h"
#include <volk.h>

namespace Pyra {

class PvImageView;
struct PvImageViewCreateInfo;

template <> struct CreateInfo_T<PvImageView> {
  using type = PvImageViewCreateInfo;
};

struct PvImageViewCreateInfo : PvInfo<VkImageViewCreateInfo, PvImageView> {

  VkImageViewCreateFlags flags;
  VkImage image;
  VkImageViewType viewType;
  VkFormat format;
  VkComponentMapping components;
  VkImageSubresourceRange subresourceRange;

  friend PvImageView;
  friend PvInfo<VkImageCreateInfo, PvImageView>;

private:
  void assign();
};

class PvImageView : public PvResource<VkImageView, PFN_vkDestroyImageView> {
public:
  bool init(PvImageViewCreateInfo &info);

  template <typename... T>
  PvImageView(PvImageViewCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }

  PvImageView(PvTable *t, VkImageView v, ManageOperation op = AUTO_MANAGE);
};

} // namespace Pyra