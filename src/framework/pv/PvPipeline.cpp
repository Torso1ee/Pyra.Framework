#include "pv/PvPipeline.h"
#include "pv/PvCommon.h"
#include <cstdint>

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

} // namespace Pyra