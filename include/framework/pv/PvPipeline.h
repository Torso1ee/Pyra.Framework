#pragma once
#include "PvResource.h"
#include "core/trait.h"
#include "pv/PvCommon.h"
#include <array>
#include <optional>
#include <vector>
#include <volk.h>
#include <vulkan/vulkan_core.h>

namespace Pyra {

class PvPipeline;
struct PvGraphicsPipelineCreateInfo;
struct PvComputePipelineCreateInfo;

template <> struct CreateInfo_T<PvPipeline> {
  using type = PvGraphicsPipelineCreateInfo;
};

struct PvPipelineShaderStageCreateInfo;
struct PvSpecializationInfo : public PvInfo<VkSpecializationInfo, PvPipeline> {
  std::vector<VkSpecializationMapEntry> mapEntrys;
  size_t dataSize;
  const void *data;
  bool required;

  friend PvInfo<VkSpecializationInfo, PvPipeline>;
  friend PvPipelineShaderStageCreateInfo;

private:
  void assign();
};

struct PvPipelineShaderStageCreateInfo
    : public PvInfo<VkPipelineShaderStageCreateInfo, PvPipeline> {
  VkPipelineShaderStageCreateFlags flags;
  VkShaderStageFlagBits stage;
  VkShaderModule module;
  const char *name;
  PvSpecializationInfo specializationInfo;
  bool required;

  friend PvInfo<VkPipelineShaderStageCreateInfo, PvPipeline>;
  friend PvGraphicsPipelineCreateInfo;
  friend PvComputePipelineCreateInfo;

private:
  void assign();
};

struct PvPipelineVertexInputStateCreateInfo
    : public PvInfo<VkPipelineVertexInputStateCreateInfo, PvPipeline> {
  VkPipelineVertexInputStateCreateFlags flags;
  std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions;
  std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions;
  bool required;

  friend PvInfo<VkPipelineVertexInputStateCreateInfo, PvPipeline>;
  friend PvGraphicsPipelineCreateInfo;

private:
  void assign();
};

struct PvPipelineInputAssemblyStateCreateInfo
    : public PvInfo<VkPipelineInputAssemblyStateCreateInfo, PvPipeline> {
  VkPipelineInputAssemblyStateCreateFlags flags;
  VkPrimitiveTopology topology;
  VkBool32 primitiveRestartEnable;
  friend PvGraphicsPipelineCreateInfo;
  friend PvInfo<VkPipelineInputAssemblyStateCreateInfo, PvPipeline>;
  bool required;

private:
  void assign();
};

struct PvPipelineTessellationStateCreateInfo
    : public PvInfo<VkPipelineTessellationStateCreateInfo, PvPipeline> {
  VkPipelineTessellationStateCreateFlags flags;
  uint32_t patchControlPoints;
  bool required;

  friend PvGraphicsPipelineCreateInfo;
  friend PvInfo<VkPipelineTessellationStateCreateInfo, PvPipeline>;

private:
  void assign();
};

struct PvPipelineViewportStateCreateInfo
    : public PvInfo<VkPipelineViewportStateCreateInfo, PvPipeline> {
  VkPipelineViewportStateCreateFlags flags;
  std::vector<VkViewport> viewports;
  std::vector<VkRect2D> scissors;
  friend PvGraphicsPipelineCreateInfo;
  friend PvInfo<VkPipelineViewportStateCreateInfo, PvPipeline>;
  bool required;

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
  bool required;

  friend PvGraphicsPipelineCreateInfo;
  friend PvInfo<VkPipelineRasterizationStateCreateInfo, PvPipeline>;

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
  bool required;

  friend PvGraphicsPipelineCreateInfo;
  friend PvInfo<VkPipelineMultisampleStateCreateInfo, PvPipeline>;
  

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
  bool required;

  friend PvGraphicsPipelineCreateInfo;
  friend PvInfo<VkPipelineDepthStencilStateCreateInfo, PvPipeline>;

private:
  void assign();
};

struct PvPipelineColorBlendStateCreateInfo
    : public PvInfo<VkPipelineColorBlendStateCreateInfo, PvPipeline> {
  VkPipelineColorBlendStateCreateFlags flags;
  VkBool32 logicOpEnable;
  VkLogicOp logicOp;
  std::vector<VkPipelineColorBlendAttachmentState> attachments;
  std::array<float, 4> blendConstants;
  bool required;

  friend PvGraphicsPipelineCreateInfo;
  friend PvInfo<VkPipelineColorBlendStateCreateInfo, PvPipeline>;

private:
  void assign();
};

struct PvPipelineDynamicStateCreateInfo
    : public PvInfo<VkPipelineDynamicStateCreateInfo, PvPipeline> {
  VkPipelineDynamicStateCreateFlags flags;
  std::vector<VkDynamicState> dynamicStates;
  bool required;

  friend PvGraphicsPipelineCreateInfo;
  friend PvInfo<VkPipelineDynamicStateCreateInfo, PvPipeline>;

private:
  void assign();
};

struct PvMultiGraphicsPipelineCreateInfo
    : public PvInfo<PvPlaceHolder, PvPipeline> {
  VkPipelineCache pipelineCache;
  std::vector<PvGraphicsPipelineCreateInfo> graphicsPipelineCreateInfos;
};

struct PvGraphicsPipelineCreateInfo
    : public PvInfo<VkGraphicsPipelineCreateInfo, PvPipeline> {
  VkPipelineCreateFlags flags;
  std::vector<PvPipelineShaderStageCreateInfo> stages;
  PvPipelineVertexInputStateCreateInfo vertexInputState;
  PvPipelineInputAssemblyStateCreateInfo inputAssemblyState;
  PvPipelineTessellationStateCreateInfo tessellationState;
  PvPipelineViewportStateCreateInfo viewportState;
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
  VkPipelineCache pipelineCache;

  friend PvPipeline;
  friend PvInfo<PvGraphicsPipelineCreateInfo, PvPipeline>;
  friend PvMultiGraphicsPipelineCreateInfo;

private:
  void assign();
};

struct PvMultiComputePipelineCreateInfo
    : public PvInfo<PvPlaceHolder, PvPipeline> {
  VkPipelineCache pipelineCache;
  std::vector<PvComputePipelineCreateInfo> computePipelineCreateInfos;
};

struct PvComputePipelineCreateInfo
    : public PvInfo<VkComputePipelineCreateInfo, PvPipeline> {
  VkPipelineCreateFlags flags;
  PvPipelineShaderStageCreateInfo stage;
  VkPipelineLayout layout;
  VkPipeline basePipelineHandle;
  int32_t basePipelineIndex;
  VkPipelineCache pipelineCache;

  friend PvPipeline;
  friend PvInfo<VkComputePipelineCreateInfo, PvPipeline>;

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

  bool init(PvMultiGraphicsPipelineCreateInfo &info);

  PvPipeline(PvMultiGraphicsPipelineCreateInfo &info) { init(info); }

  bool init(PvMultiComputePipelineCreateInfo &info);

  PvPipeline(PvMultiComputePipelineCreateInfo &info) { init(info); }
};

} // namespace Pyra