#include "app/RenderContext.h"
#include "app/VulkanApplication.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "pv/PvImageView.h"
#include "pv/PvPipeline.h"
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
    contextData.renderPass = bootstrap->make<PvRenderPass>(info);
  }

  void createFramebuffers() override {
    contextData.framebuffers.resize(swapChainData->images.size());
    for (uint32_t i = 0; i < swapChainData->images.size(); i++) {
      CreateInfo<PvFramebuffer> info{
          .renderPass = contextData.renderPass->handle,
          .attachments = {swapChainData->imageViews[i]->handle},
          .width = swapChainData->swapchain->extent.width,
          .height = swapChainData->swapchain->extent.height,
          .layers = 1};
      contextData.framebuffers[i] = bootstrap->make<PvFramebuffer>(info);
    }
  }

  void createPipeline() override {
    // TODO:shader
    CreateInfo<PvPipeline> info{
        .stages =
            {
                {.stage = VK_SHADER_STAGE_VERTEX_BIT, .name = "main"},
                {.stage = VK_SHADER_STAGE_FRAGMENT_BIT, .name = "main"},
            },
        .vertexInputState = {},
        .inputAssemblyState =
            {

            },
        .viewportState =
            {

            },
        .rasterizationState =
            {

            },
        .multisampleState =
            {

            },
        .colorBlendState = {.blendConstants = {1, 3, 4, 5}},
        .dynamicState =
            {

            },
        .renderPass = contextData.renderPass->handle,
        .subpass = 0,
        .basePipelineHandle = nullptr};
    contextData.pipeline = bootstrap->make<PvPipeline>(info);
  }
};

int main(int, char **) {
  VulkanApplication app;
  app.addContext<TriangleRenderContext>();
  app.run();
  return 0;
}