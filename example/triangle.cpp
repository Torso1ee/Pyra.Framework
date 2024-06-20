#include "app/RenderContext.h"
#include "app/VulkanApplication.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "pv/PvImageView.h"
#include "pv/PvRenderPass.h"
#include <memory>

using namespace Pyra;

class TriangleRenderContext : public RenderContext<ContextData> {
  void createRenderPass() override {
    CreateInfo<PvRenderPass> info{
        .attachments = {{.format = swapChainData->swapchain->image_format,
                         .samples = VK_SAMPLE_COUNT_1_BIT,
                         .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                         .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                         .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                         .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                         .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                         .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR}},
        .subpasses = {{.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                       .ColorAttachments =
                           {{.attachment = 0,
                             .layout =
                                 VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}}}},
        .dependencies = {
            {.srcSubpass = VK_SUBPASS_EXTERNAL,
             .dstSubpass = 0,
             .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
             .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
             .srcAccessMask = 0,
             .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                              VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT}}};
    renderPass = bootstrap->make<PvRenderPass>(info);
  }

  void createFramebuffers() override {
    framebuffers.resize(swapChainData->images.size());
    for (uint32_t i = 0; i < swapChainData->images.size(); i++) {
      CreateInfo<PvFramebuffer> info{
          .renderPass = renderPass->handle,
          .attachments = {swapChainData->imageViews[i]->handle},
          .width = swapChainData->swapchain->extent.width,
          .height = swapChainData->swapchain->extent.height,
          .layers = 1};
      framebuffers[i] = bootstrap->make<PvFramebuffer>(info);
    }
  }
};

int main(int, char **) {
  VulkanApplication app;
  app.addContext<TriangleRenderContext>();
  app.run();
  return 0;
}