#include "pv/PvPipeline.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <vector>

namespace Pyra {

void PvSpecializationInfo::assign() {
  info = {
      .mapEntryCount = (uint32_t)mapEntrys.size(),
      .pMapEntries = NULLPTR_IF_EMPTY(mapEntrys),
      .dataSize = dataSize,
      .pData = data,
  };
}

void PvPipelineShaderStageCreateInfo::assign() {
  specializationInfo.assign();
  info = {
      .flags = flags,
      .stage = stage,
      .module = module,
      .pName = name,
      .pSpecializationInfo = &specializationInfo.info,
  };
}

VkPipelineVertexInputStateCreateFlags flags;
uint32_t vertexBindingDescriptionCount;
const VkVertexInputBindingDescription *pVertexBindingDescriptions;
uint32_t vertexAttributeDescriptionCount;
const VkVertexInputAttributeDescription *pVertexAttributeDescriptions;

void PvPipelineVertexInputStateCreateInfo::assign() {
  info = {
      .flags = flags,
      .vertexBindingDescriptionCount =
          (uint32_t)vertexBindingDescriptions.size(),
      .pVertexBindingDescriptions = NULLPTR_IF_EMPTY(vertexBindingDescriptions),
      .vertexAttributeDescriptionCount =
          (uint32_t)vertexAttributeDescriptions.size(),
      .pVertexAttributeDescriptions =
          NULLPTR_IF_EMPTY(vertexAttributeDescriptions),
  };
}

void PvPipelineInputAssemblyStateCreateInfo::assign() {
  info = {.flags = flags,
          .topology = topology,
          .primitiveRestartEnable = primitiveRestartEnable};
}

void PvPipelineTessellationStateCreateInfo::assign() {
  info = {.flags = flags, .patchControlPoints = patchControlPoints};
}

void PvPipelineViewportStateCreateInfo::assign() {
  info = {.flags = flags,
          .viewportCount = (uint32_t)viewports.size(),
          .pViewports = NULLPTR_IF_EMPTY(viewports),
          .scissorCount = (uint32_t)scissors.size(),
          .pScissors = NULLPTR_IF_EMPTY(scissors)};
}

void PvPipelineRasterizationStateCreateInfo::assign() {
  info = {.flags = flags,
          .depthClampEnable = depthClampEnable,
          .rasterizerDiscardEnable = rasterizerDiscardEnable,
          .polygonMode = polygonMode,
          .cullMode = cullMode,
          .frontFace = frontFace,
          .depthBiasEnable = depthBiasEnable,
          .depthBiasConstantFactor = depthBiasConstantFactor,
          .depthBiasClamp = depthBiasClamp,
          .depthBiasSlopeFactor = depthBiasSlopeFactor,
          .lineWidth = lineWidth};
}

void PvPipelineMultisampleStateCreateInfo::assign() {
  info = {.flags = flags,
          .rasterizationSamples = rasterizationSamples,
          .sampleShadingEnable = sampleShadingEnable,
          .minSampleShading = minSampleShading,
          .pSampleMask = NULLPTR_IF_EMPTY(sampleMasks),
          .alphaToCoverageEnable = alphaToCoverageEnable,
          .alphaToOneEnable = alphaToOneEnable};
}

void PvPipelineDepthStencilStateCreateInfo::assign() {
  info = {.flags = flags,
          .depthTestEnable = depthTestEnable,
          .depthWriteEnable = depthWriteEnable,
          .depthCompareOp = depthCompareOp,
          .depthBoundsTestEnable = depthBoundsTestEnable,
          .stencilTestEnable = stencilTestEnable,
          .front = front,
          .back = back,
          .minDepthBounds = minDepthBounds,
          .maxDepthBounds = maxDepthBounds};
}

void PvPipelineColorBlendStateCreateInfo::assign() {
  info = {.flags = flags,
          .logicOpEnable = logicOpEnable,
          .logicOp = logicOp,
          .attachmentCount = (uint32_t)attachments.size(),
          .pAttachments = NULLPTR_IF_EMPTY(attachments),
          .blendConstants = {blendConstants[0], blendConstants[1],
                             blendConstants[2], blendConstants[3]}};
}

void PvPipelineDynamicStateCreateInfo::assign() {
  info = {.flags = flags,
          .dynamicStateCount = (uint32_t)dynamicStates.size(),
          .pDynamicStates = NULLPTR_IF_EMPTY(dynamicStates)};
}

void PvGraphicsPipelineCreateInfo::assign() {
  std::vector<VkPipelineShaderStageCreateInfo> sInfos{};
  for (auto &stage : stages) {
    stage.assign();
    sInfos.push_back(stage.info);
  }
  if (vertexInputState.has_value())
    vertexInputState->assign();
  if (inputAssemblyState.has_value())
    inputAssemblyState->assign();
  if (tessellationState.has_value())
    tessellationState->assign();
  if (viewportState.has_value())
    viewportState->assign();
  if (rasterizationState.has_value())
    rasterizationState->assign();
  if (multisampleState.has_value())
    multisampleState->assign();
  if (depthStencilState.has_value())
    depthStencilState->assign();
  if (colorBlendState.has_value())
    colorBlendState->assign();
  if (dynamicState.has_value())
    dynamicState->assign();
  info = {
      .flags = flags,
      .stageCount = (uint32_t)sInfos.size(),
      .pStages = NULLPTR_IF_EMPTY(sInfos),
      .pVertexInputState =
          vertexInputState.has_value() ? &vertexInputState->info : nullptr,
      .pInputAssemblyState =
          inputAssemblyState.has_value() ? &inputAssemblyState->info : nullptr,
      .pTessellationState =
          tessellationState.has_value() ? &tessellationState->info : nullptr,
      .pViewportState =
          viewportState.has_value() ? &viewportState->info : nullptr,
      .pRasterizationState =
          rasterizationState.has_value() ? &rasterizationState->info : nullptr,
      .pMultisampleState =
          multisampleState.has_value() ? &multisampleState->info : nullptr,
      .pDepthStencilState =
          depthStencilState.has_value() ? &depthStencilState->info : nullptr,
      .pColorBlendState =
          colorBlendState.has_value() ? &colorBlendState->info : nullptr,
      .pDynamicState = dynamicState.has_value() ? &dynamicState->info : nullptr,
      .layout = layout,
      .renderPass = renderPass,
      .subpass = subpass,
      .basePipelineHandle = basePipelineHandle,
      .basePipelineIndex = basePipelineIndex};
}

void PvComputePipelineCreateInfo::assign() {
  stage.assign();
  info = {.flags = flags,
          .stage = stage.info,
          .layout = layout,
          .basePipelineHandle = basePipelineHandle,
          .basePipelineIndex = basePipelineIndex};
}

bool PvPipeline::init(PvGraphicsPipelineCreateInfo &info) {
  if (info.table->disp.createGraphicsPipelines(info.pipelineCache, 1,
                                               &info.info, info.callback,
                                               &handle) != VK_SUCCESS) {
    ERROR("Failed to create vkpipeline!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = info.table->disp.fp_vkDestroyPipeline;
  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation);
  return true;
}

bool PvPipeline::init(PvComputePipelineCreateInfo &info) {
  if (info.table->disp.createComputePipelines(info.pipelineCache, 1, &info.info,
                                              info.callback,
                                              &handle) != VK_SUCCESS) {
    ERROR("Failed to create vkpipeline!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = info.table->disp.fp_vkDestroyPipeline;
  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation);
  return true;
}

bool PvPipeline::init(PvMultiGraphicsPipelineCreateInfo &info) {
  std::vector<VkGraphicsPipelineCreateInfo> pInfos;
  for (auto &pInfo : info.graphicsPipelineCreateInfos) {
    pInfo.assign();
    pInfos.push_back(pInfo.info);
  }
  if (info.table->disp.createGraphicsPipelines(
          info.pipelineCache, pInfos.size(), NULLPTR_IF_EMPTY(pInfos),
          info.callback, &handle) != VK_SUCCESS) {
    ERROR("Failed to create vkpipeline!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = info.table->disp.fp_vkDestroyPipeline;
  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation);
  return true;
}

bool PvPipeline::init(PvMultiComputePipelineCreateInfo &info) {
  std::vector<VkComputePipelineCreateInfo> pInfos;
  for (auto &pInfo : info.computePipelineCreateInfos) {
    pInfo.assign();
    pInfos.push_back(pInfo.info);
  }
  if (info.table->disp.createComputePipelines(
          info.pipelineCache, pInfos.size(), NULLPTR_IF_EMPTY(pInfos),
          info.callback, &handle) != VK_SUCCESS) {
    ERROR("Failed to create vkpipeline!");
    return false;
  }
  if (deconstuctor == nullptr)
    deconstuctor = info.table->disp.fp_vkDestroyPipeline;
  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation);
  return true;
}

} // namespace Pyra