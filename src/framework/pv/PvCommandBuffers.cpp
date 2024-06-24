#include "pv/PvCommandBuffers.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <vector>
#include <volk.h>
#include <vulkan/vulkan_core.h>

namespace Pyra {

void PvCommandBuffersAllocateInfo::assign() {
  info = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .commandPool = commandPool,
      .level = level,
      .commandBufferCount = commandBufferCount,
  };
}

bool PvCommandBuffers::init(PvCommandBuffersAllocateInfo &info) {
  table = info.table;
  commandBuffers = new std::vector<VkCommandBuffer>{info.commandBufferCount};
  if (info.table->disp.allocateCommandBuffers(
          &info.info, commandBuffers->data()) != VK_SUCCESS) {
    ERROR("Failed to create vkCommandBuffer!");
    return false;
  }
  handle = commandBuffers->data();
  if (deconstuctor == nullptr)
    deconstuctor = info.table->disp.fp_vkFreeCommandBuffers;
  manage(handle,
         std::make_tuple(info.table->device.device, info.commandPool,
                         info.commandBufferCount, handle),
         info.operation);
  return true;
}

void BeginCommandBufferInfo::assign() {
  inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
  info = {.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
          .flags = flags,
          .pInheritanceInfo =
              inheritanceInfo.required ? &inheritanceInfo : nullptr};
}

void RenderPassBeginInfo::assign() {
  info = {
      .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
      .renderPass = renderPass,
      .framebuffer = framebuffer,
      .clearValueCount = (uint32_t)clearValues.size(),
      .pClearValues = NULLPTR_IF_EMPTY(clearValues)
  };
}

VkResult PvCommandBuffer::beginCommandBuffer(BeginCommandBufferInfo info) {
  info.assign();
  return table->disp.beginCommandBuffer(commandBuffer, &info.info);
}

void PvCommandBuffer::setViewport(SetViewportInfo info) {
  table->disp.cmdSetViewport(commandBuffer, info.firstViewport,
                             info.viewports.size(),
                             NULLPTR_IF_EMPTY(info.viewports));
}

void PvCommandBuffer::setScissor(SetScissorInfo info) {
  table->disp.cmdSetScissor(commandBuffer, info.firstScissor,
                            info.scissors.size(),
                            NULLPTR_IF_EMPTY(info.scissors));
}

void PvCommandBuffer::beginRenderPass(RenderPassBeginInfo info) {
  info.assign();
  table->disp.cmdBeginRenderPass(commandBuffer, &info.info,
                                 info.subpassContents);
}

void PvCommandBuffer::bindPipeline(BindPipelineInfo info) {
  table->disp.cmdBindPipeline(commandBuffer, info.pipelineBindPoint,
                              info.pipeline);
}

void PvCommandBuffer::draw(DrawInfo info) {
  table->disp.cmdDraw(commandBuffer, info.vertexCount, info.instanceCount,
                      info.firstVertex, info.instanceCount);
}

void PvCommandBuffer::endRenderPass() {
  table->disp.cmdEndRenderPass(commandBuffer);
}

VkResult PvCommandBuffer::endCommandBuffer() {
  return table->disp.endCommandBuffer(commandBuffer);
}

} // namespace Pyra