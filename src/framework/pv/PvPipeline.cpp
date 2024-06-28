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
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .flags = flags,
      .stage = stage,
      .module = module,
      .pName = name,
      .pSpecializationInfo = &specializationInfo.info,
  };
}

void PvPipelineVertexInputStateCreateInfo::assign() {
  info = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
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
  info = {.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
          .flags = flags,
          .topology = topology,
          .primitiveRestartEnable = primitiveRestartEnable};
}

void PvPipelineTessellationStateCreateInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
          .flags = flags,
          .patchControlPoints = patchControlPoints};
}

void PvPipelineViewportStateCreateInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
          .flags = flags,
          .viewportCount = (uint32_t)viewports.size(),
          .pViewports = NULLPTR_IF_EMPTY(viewports),
          .scissorCount = (uint32_t)scissors.size(),
          .pScissors = NULLPTR_IF_EMPTY(scissors)};
}

void PvPipelineRasterizationStateCreateInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
          .flags = flags,
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
  info = {.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
          .flags = flags,
          .rasterizationSamples = rasterizationSamples,
          .sampleShadingEnable = sampleShadingEnable,
          .minSampleShading = minSampleShading,
          .pSampleMask = NULLPTR_IF_EMPTY(sampleMasks),
          .alphaToCoverageEnable = alphaToCoverageEnable,
          .alphaToOneEnable = alphaToOneEnable};
}

void PvPipelineDepthStencilStateCreateInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
          .flags = flags,
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
  info = {.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
          .flags = flags,
          .logicOpEnable = logicOpEnable,
          .logicOp = logicOp,
          .attachmentCount = (uint32_t)attachments.size(),
          .pAttachments = NULLPTR_IF_EMPTY(attachments),
          .blendConstants = {blendConstants[0], blendConstants[1],
                             blendConstants[2], blendConstants[3]}};
}

void PvPipelineDynamicStateCreateInfo::assign() {
  info = {.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
          .flags = flags,
          .dynamicStateCount = (uint32_t)dynamicStates.size(),
          .pDynamicStates = NULLPTR_IF_EMPTY(dynamicStates)};
}

void PvGraphicsPipelineCreateInfo::assign() {
  for (auto &stage : stages) {
    stage.assign();
  }
  if (vertexInputState.required)
    vertexInputState.assign();
  if (inputAssemblyState.required)
    inputAssemblyState.assign();
  if (tessellationState.required)
    tessellationState.assign();
  if (viewportState.required)
    viewportState.assign();
  if (rasterizationState.required)
    rasterizationState.assign();
  if (multisampleState.required)
    multisampleState.assign();
  if (depthStencilState.required)
    depthStencilState.assign();
  if (colorBlendState.required)
    colorBlendState.assign();
  if (dynamicState.required)
    dynamicState.assign();
  info = {.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
          .flags = flags,

          .pVertexInputState =
              vertexInputState.required ? &vertexInputState.info : nullptr,
          .pInputAssemblyState =
              inputAssemblyState.required ? &inputAssemblyState.info : nullptr,
          .pTessellationState =
              tessellationState.required ? &tessellationState.info : nullptr,
          .pViewportState =
              viewportState.required ? &viewportState.info : nullptr,
          .pRasterizationState =
              rasterizationState.required ? &rasterizationState.info : nullptr,
          .pMultisampleState =
              multisampleState.required ? &multisampleState.info : nullptr,
          .pDepthStencilState =
              depthStencilState.required ? &depthStencilState.info : nullptr,
          .pColorBlendState =
              colorBlendState.required ? &colorBlendState.info : nullptr,
          .pDynamicState = dynamicState.required ? &dynamicState.info : nullptr,
          .layout = layout,
          .renderPass = renderPass,
          .subpass = subpass,
          .basePipelineHandle = basePipelineHandle,
          .basePipelineIndex = basePipelineIndex};
}

void PvComputePipelineCreateInfo::assign() {
  stage.assign();
  info = {.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
          .flags = flags,
          .stage = stage.info,
          .layout = layout,
          .basePipelineHandle = basePipelineHandle,
          .basePipelineIndex = basePipelineIndex};
}

bool PvPipeline::init(PvGraphicsPipelineCreateInfo &info) {
  table = info.table;
  std::vector<VkPipelineShaderStageCreateInfo> sInfos{};
  for (auto &stage : info.stages) {
    sInfos.push_back(stage.info);
  }
  info.info.stageCount = (uint32_t)sInfos.size();
  info.info.pStages = NULLPTR_IF_EMPTY(sInfos);
  if (info.table->disp.createGraphicsPipelines(info.pipelineCache, 1,
                                               &info.info, info.callback,
                                               &handle) != VK_SUCCESS) {
    ERROR("Failed to create vkpipeline!");
    return false;
  }
  if (!setDctor)
    setDeconstructor(info.table->disp.fp_vkDestroyPipeline);
  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation,
         {info.table->device.device, info.pipelineCache, info.layout,
          info.renderPass});
  return true;
}

bool PvPipeline::init(PvComputePipelineCreateInfo &info) {
  table = info.table;
  if (info.table->disp.createComputePipelines(info.pipelineCache, 1, &info.info,
                                              info.callback,
                                              &handle) != VK_SUCCESS) {
    ERROR("Failed to create vkpipeline!");
    return false;
  }
  if (!setDctor)
    setDeconstructor(info.table->disp.fp_vkDestroyPipeline);
  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation,
         {info.table->device.device, info.pipelineCache, info.layout});
  return true;
}

bool PvPipeline::init(PvMultiGraphicsPipelineCreateInfo &info) {
  table = info.table;
  std::vector<VkGraphicsPipelineCreateInfo> pInfos;
  std::set<void *> depsSet{info.table->device.device, info.pipelineCache};
  for (auto &pInfo : info.graphicsPipelineCreateInfos) {
    pInfo.assign();
    pInfos.push_back(pInfo.info);
    depsSet.insert(pInfo.renderPass);
    depsSet.insert(pInfo.layout);
  }
  if (info.table->disp.createGraphicsPipelines(
          info.pipelineCache, pInfos.size(), NULLPTR_IF_EMPTY(pInfos),
          info.callback, &handle) != VK_SUCCESS) {
    ERROR("Failed to create vkpipeline!");
    return false;
  }

  std::vector<void *> deps{};
  for (auto p : depsSet)
    deps.push_back(p);

  if (!setDctor)
    setDeconstructor(info.table->disp.fp_vkDestroyPipeline);

  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation, deps);
  return true;
}

bool PvPipeline::init(PvMultiComputePipelineCreateInfo &info) {
  table = info.table;
  std::vector<VkComputePipelineCreateInfo> pInfos;
  std::set<void *> depsSet{info.table->device.device, info.pipelineCache};
  for (auto &pInfo : info.computePipelineCreateInfos) {
    pInfo.assign();
    pInfos.push_back(pInfo.info);
    depsSet.insert(pInfo.layout);
  }
  if (info.table->disp.createComputePipelines(
          info.pipelineCache, pInfos.size(), NULLPTR_IF_EMPTY(pInfos),
          info.callback, &handle) != VK_SUCCESS) {
    ERROR("Failed to create vkpipeline!");
    return false;
  }

  std::vector<void *> deps{};
  for (auto p : depsSet)
    deps.push_back(p);

  if (!setDctor)
    setDeconstructor(info.table->disp.fp_vkDestroyPipeline);

  manage(handle,
         std::make_tuple(info.table->device.device, handle, info.callback),
         info.operation, deps);
  return true;
}

} // namespace Pyra