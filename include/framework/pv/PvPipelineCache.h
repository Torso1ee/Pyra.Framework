#pragma once
#include "PvResource.h"
#include "pv/PvCommon.h"
#include <volk.h>

namespace Pyra {

class PvPipelineCache;
struct PvPipelineCacheCreateInfo;

template <> struct CreateInfo_T<PvPipelineCache> {
  using type = PvPipelineCacheCreateInfo;
};

struct PvPipelineCacheCreateInfo
    : PvInfo<VkPipelineCacheCreateInfo, PvPipelineCache> {
  VkPipelineCacheCreateFlags flags;
  size_t initialDataSize;
  const void *pInitialData;

  friend PvPipelineCache;
  friend PvInfo<VkPipelineCacheCreateInfo, PvPipelineCache>;

private:
  void assign();
};

class PvPipelineCache
    : public PvResource<VkPipelineCache, PFN_vkDestroyPipelineCache> {
public:
  bool init(PvPipelineCacheCreateInfo &info);

  template <typename... T>
  PvPipelineCache(PvPipelineCacheCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }
};

} // namespace Pyra