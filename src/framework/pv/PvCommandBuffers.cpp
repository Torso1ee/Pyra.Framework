#include "pv/PvCommandBuffers.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "core/Core.h"
#include <cstdint>
#include <vector>
#include <volk.h>

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

  if (!setDctor)
    setDeconstructor(info.table->disp.fp_vkFreeCommandBuffers);

  manage(handle,
         std::make_tuple(info.table->device.device, info.commandPool,
                         info.commandBufferCount, handle),
         info.operation, {info.table->device.device, info.commandPool});
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
  info = {.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
          .renderPass = renderPass,
          .framebuffer = framebuffer,
          .clearValueCount = (uint32_t)clearValues.size(),
          .pClearValues = NULLPTR_IF_EMPTY(clearValues)};
}

PvCommandBuffer &
PvCommandBuffer::beginCommandBuffer(BeginCommandBufferInfo info) {
  info.assign();
  table->disp.beginCommandBuffer(handle, &info.info);
  return *this;
}

PvCommandBuffer &PvCommandBuffer::setViewport(SetViewportInfo info) {
  table->disp.cmdSetViewport(handle, info.firstViewport,
                             info.viewports.size(),
                             NULLPTR_IF_EMPTY(info.viewports));
  return *this;
}

PvCommandBuffer &PvCommandBuffer::setScissor(SetScissorInfo info) {
  table->disp.cmdSetScissor(handle, info.firstScissor,
                            info.scissors.size(),
                            NULLPTR_IF_EMPTY(info.scissors));
  return *this;
}

PvCommandBuffer &PvCommandBuffer::beginRenderPass(RenderPassBeginInfo info) {
  info.assign();
  table->disp.cmdBeginRenderPass(handle, &info.info,
                                 info.subpassContents);
  return *this;
}

PvCommandBuffer &PvCommandBuffer::bindPipeline(BindPipelineInfo info) {
  table->disp.cmdBindPipeline(handle, info.pipelineBindPoint,
                              info.pipeline);
  return *this;
}

PvCommandBuffer &PvCommandBuffer::draw(DrawInfo info) {
  table->disp.cmdDraw(handle, info.vertexCount, info.instanceCount,
                      info.firstVertex, info.instanceCount);
  return *this;
}

PvCommandBuffer &PvCommandBuffer::endRenderPass() {
  table->disp.cmdEndRenderPass(handle);
  return *this;
}

PvCommandBuffer &PvCommandBuffer::endCommandBuffer() {
  table->disp.endCommandBuffer(handle);
  return *this;
}

} // namespace Pyra