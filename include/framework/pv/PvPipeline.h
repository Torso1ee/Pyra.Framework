#pragma once
#include "PvResource.h"
#include "pv/PvCommon.h"
#include <vector>
#include <volk.h>

namespace Pyra {

class PvPipeline;
struct PvGraphicsPipelineCreateInfo;

template <> struct CreateInfo_T<PvPipeline> {
  using type = PvGraphicsPipelineCreateInfo;
};

struct PvSpecializationInfo {
  std::vector<VkSpecializationMapEntry> mapEntrys;
  size_t dataSize;
  const void *data;
};

struct PvPipelineShaderStageCreateInfo
    : public PvInfo<VkPipelineShaderStageCreateInfo, PvPipeline> {
  VkPipelineShaderStageCreateFlags flags;
  VkShaderStageFlagBits stage;
  VkShaderModule module;
  const char *name;
  PvSpecializationInfo specializationInfo;

  friend PvInfo<VkPipelineShaderStageCreateInfo, PvPipeline>;

private:
  void assign();
};

struct PvPipelineVertexInputStateCreateInfo
    : public PvInfo<VkPipelineVertexInputStateCreateInfo, PvPipeline> {
  VkPipelineVertexInputStateCreateFlags flags;
  std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions;
  std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions;

  friend PvInfo<VkPipelineVertexInputStateCreateInfo, PvPipeline>;

private:
  void assign();
};

struct PvPipelineInputAssemblyStateCreateInfo
    : public PvInfo<VkPipelineInputAssemblyStateCreateInfo, PvPipeline> {
  VkPipelineInputAssemblyStateCreateFlags flags;
  VkPrimitiveTopology topology;
  VkBool32 primitiveRestartEnable;

private:
  void assign();
};

struct PvPipelineTessellationStateCreateInfo
    : public PvInfo<VkPipelineTessellationStateCreateInfo, PvPipeline> {
  VkPipelineTessellationStateCreateFlags flags;
  uint32_t patchControlPoints;

private:
  void assign();
};

struct PvPipelineViewportStateCreateInfo
    : public PvInfo<VkPipelineViewportStateCreateInfo, PvPipeline> {
  VkPipelineViewportStateCreateFlags flags;
  std::vector<VkViewport> viewports;
  std::vector<VkRect2D> scissors;

private:
  void assign();
};

struct PvPipelineRasterizationStateCreateInfo
    : public PvInfo<VkPipelineRasterizationStateCreateInfo, PvPipeline> {
  VkPipelineRasterizationStateCreateFlags flags;
  VkBool32 depthClampEnable;
  VkBool32 rasterizerDiscardEnable;
  VkPolygonMode polygonMode;
  VkCullModeFlags cullMode;
  VkFrontFace frontFace;
  VkBool32 depthBiasEnable;
  float depthBiasConstantFactor;
  float depthBiasClamp;
  float depthBiasSlopeFactor;
  float lineWidth;

private:
  void assign();
};

struct PvPipelineMultisampleStateCreateInfo
    : public PvInfo<VkPipelineMultisampleStateCreateInfo, PvPipeline> {
  VkPipelineMultisampleStateCreateFlags flags;
  VkSampleCountFlagBits rasterizationSamples;
  VkBool32 sampleShadingEnable;
  float minSampleShading;
  std::vector<VkSampleMask> sampleMasks;
  VkBool32 alphaToCoverageEnable;
  VkBool32 alphaToOneEnable;

private:
  void assign();
};

struct PvPipelineDepthStencilStateCreateInfo
    : public PvInfo<VkPipelineDepthStencilStateCreateInfo, PvPipeline> {
  VkPipelineDepthStencilStateCreateFlags flags;
  VkBool32 depthTestEnable;
  VkBool32 depthWriteEnable;
  VkCompareOp depthCompareOp;
  VkBool32 depthBoundsTestEnable;
  VkBool32 stencilTestEnable;
  VkStencilOpState front;
  VkStencilOpState back;
  float minDepthBounds;
  float maxDepthBounds;

private:
  void assign();
};

struct PvPipelineColorBlendStateCreateInfo
    : public PvInfo<VkPipelineColorBlendStateCreateInfo, PvPipeline> {
  VkPipelineColorBlendStateCreateFlags flags;
  VkBool32 logicOpEnable;
  VkLogicOp logicOp;
  std::vector<VkPipelineColorBlendAttachmentState> attachments;
  float blendConstants[4];

private:
  void assign();
};

struct PvPipelineDynamicStateCreateInfo
    : public PvInfo<VkPipelineDynamicStateCreateInfo, PvPipeline> {
  VkPipelineDynamicStateCreateFlags flags;
  std::vector<VkDynamicState> dynamicStates;

private:
  void assign();
};

struct PvGraphicsPipelineCreateInfo
    : public PvInfo<VkGraphicsPipelineCreateInfo, PvPipeline> {
  VkPipelineCreateFlags flags;
  std::vector<PvPipelineShaderStageCreateInfo> stages;
  PvPipelineVertexInputStateCreateInfo vertexInputState;
  PvPipelineInputAssemblyStateCreateInfo inputAssemblyState;
  PvPipelineTessellationStateCreateInfo tessellationState;
  PvPipelineRasterizationStateCreateInfo rasterizationState;
  PvPipelineMultisampleStateCreateInfo multisampleState;
  PvPipelineDepthStencilStateCreateInfo depthStencilState;
  PvPipelineColorBlendStateCreateInfo colorBlendState;
  PvPipelineDynamicStateCreateInfo dynamicState;
  VkPipelineLayout layout;
  VkRenderPass renderPass;
  uint32_t subpass;
  VkPipeline basePipelineHandle;
  int32_t basePipelineIndex;

  friend PvPipeline;
  friend PvInfo<PvGraphicsPipelineCreateInfo, PvPipeline>;

private:
  void assign();
};

struct PvComputePipelineCreateInfo
    : public PvInfo<VkComputePipelineCreateInfo, PvPipeline> {
  VkPipelineCreateFlags flags;
  VkPipelineShaderStageCreateInfo stage;
  VkPipelineLayout layout;
  VkPipeline basePipelineHandle;
  int32_t basePipelineIndex;

  friend PvPipeline;
  friend PvInfo<VkRenderPassCreateInfo, PvPipeline>;

private:
  void assign();
};

class PvPipeline : public PvResource<VkPipeline, PFN_vkDestroyPipeline> {
public:
  bool init(PvGraphicsPipelineCreateInfo &info);

  template <typename... T>
  PvPipeline(PvGraphicsPipelineCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }

  bool init(PvComputePipelineCreateInfo &info);

  template <typename... T>
  PvPipeline(PvComputePipelineCreateInfo &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  }
};

} // namespace Pyra