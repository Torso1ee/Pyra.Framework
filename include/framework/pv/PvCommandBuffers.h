#pragma once
#include "pv/PvCommon.h"
#include "pv/PvResource.h"
#include <cstddef>
#include <cstdint>
#include <vector>
#include <volk.h>
namespace Pyra {

class PvCommandBuffer;
class PvCommandBuffers;
class PvCommandPool;
struct PvCommandBuffersAllocateInfo;

template <> struct CreateInfo_T<PvCommandBuffers> {
  using type = PvCommandBuffersAllocateInfo;
};

struct PvCommandBuffersAllocateInfo
    : PvInfo<VkCommandBufferAllocateInfo, PvCommandBuffers> {

  VkCommandPool commandPool;
  VkCommandBufferLevel level;
  uint32_t commandBufferCount;

  friend PvCommandBuffers;
  friend PvCommandPool;
  friend PvInfo<VkCommandBufferAllocateInfo, PvCommandBuffers>;

private:
  void assign();
};

struct BeginCommandBufferInfo
    : PvInfo<VkCommandBufferBeginInfo, PvCommandBuffers> {

  VkCommandBufferUsageFlags flags;
  MemberInfo<VkCommandBufferInheritanceInfo> inheritanceInfo;

  friend PvCommandBuffer;
  friend PvInfo<VkCommandBufferBeginInfo, PvCommandBuffers>;

private:
  void assign();
};

struct SetViewportInfo {
  uint32_t firstViewport;
  std::vector<VkViewport> viewports;
};

struct SetScissorInfo {
  uint32_t firstScissor;
  std::vector<VkRect2D> scissors;
};

struct BindPipelineInfo {
  VkPipelineBindPoint pipelineBindPoint;
  VkPipeline pipeline;
};

struct DrawInfo {
  uint32_t vertexCount;
  uint32_t instanceCount;
  uint32_t firstVertex;
  uint32_t firstInstance;
};

struct RenderPassBeginInfo : PvInfo<VkRenderPassBeginInfo, PvCommandBuffers> {

  VkRenderPass renderPass;
  VkFramebuffer framebuffer;
  VkRect2D renderArea;
  std::vector<VkClearValue> clearValues;
  VkSubpassContents subpassContents;

  friend PvCommandBuffer;
  friend PvInfo<VkRenderPassBeginInfo, PvCommandBuffers>;

private:
  void assign();
};

struct PvCommandBuffer {

  PvTable *table;
  VkCommandBuffer handle;
  PvCommandBuffers *parent;

  ~PvCommandBuffer();

  PvCommandBuffer &reset(VkCommandBufferResetFlags flag = 0);

  PvCommandBuffer &beginCommandBuffer(BeginCommandBufferInfo info);

  PvCommandBuffer &setViewport(SetViewportInfo info);

  PvCommandBuffer &setScissor(SetScissorInfo info);

  PvCommandBuffer &beginRenderPass(RenderPassBeginInfo info);

  PvCommandBuffer &bindPipeline(BindPipelineInfo info);

  PvCommandBuffer &draw(DrawInfo info);

  PvCommandBuffer &endRenderPass();

  PvCommandBuffer &endCommandBuffer();
};

class PvCommandBuffers
    : public PvResource<VkCommandBuffer *, PFN_vkFreeCommandBuffers> {
public:
  bool init(PvCommandBuffersAllocateInfo &info);
  bool active = false;

  template <typename... T>
  PvCommandBuffers(PvCommandBuffersAllocateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }

  PvCommandBuffer get(uint32_t i) {
    return {.table = table, .handle = commandBuffers->at(i), .parent = this};
  }

  size_t size() { return commandBuffers->size(); }

private:
  std::vector<VkCommandBuffer> *commandBuffers;
};

} // namespace Pyra