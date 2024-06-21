#include "app/RenderContext.h"
#include "app/VulkanApplication.h"
#include "pv/PvBootstrap.h"
#include "pv/PvCommon.h"
#include "pv/PvImageView.h"
#include "pv/PvPipeline.h"
#include "pv/PvPipelineLayout.h"
#include "pv/PvRenderPass.h"
#include "pv/PvShaderModule.h"
#include "vulkan/vulkan_core.h"
#include <memory>

using namespace Pyra;

class TriangleRenderContext : public RenderContext<ContextData> {
  void createRenderPass() override {
    CreateInfo<PvRenderPass> info{
        .attachments = {{.format = swapchainData->swapchain->image_format,
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
    contextData.framebuffers.resize(swapchainData->images.size());
    for (uint32_t i = 0; i < swapchainData->images.size(); i++) {
      CreateInfo<PvFramebuffer> info{
          .renderPass = contextData.renderPass->handle,
          .attachments = {swapchainData->imageViews[i]->handle},
          .width = swapchainData->swapchain->extent.width,
          .height = swapchainData->swapchain->extent.height,
          .layers = 1};
      contextData.framebuffers[i] = bootstrap->make<PvFramebuffer>(info);
    }
  }

  void createPipeline() override {
    CreateInfo<PvPipelineLayout> plInfo;
    contextData.pipelineLayout = bootstrap->make<PvPipelineLayout>(plInfo);
    const char *vert =
#include "generated/triangle.vert"
        ;
    auto vertModule =
        bootstrap->createShaderModule(vert, shaderc_vertex_shader);
    const char *frag =
#include "generated/triangle.frag"
        ;
    auto fragModule =
        bootstrap->createShaderModule(frag, shaderc_fragment_shader);
    CreateInfo<PvPipeline> info{
        .stages =
            {
                {.stage = VK_SHADER_STAGE_VERTEX_BIT,
                 .module = vertModule->handle,
                 .name = "main"},
                {.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
                 .module = fragModule->handle,
                 .name = "main"},
            },
        .vertexInputState =
            {
                .required = true,
            },
        .inputAssemblyState =
            {
                .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                .primitiveRestartEnable = VK_FALSE,
                .required = true,
            },
        .viewportState =
            {.viewports = {{.x = 0.0f,
                            .y = 0.0f,
                            .width =
                                (float)swapchainData->swapchain->extent.width,
                            .height =
                                (float)swapchainData->swapchain->extent.height,
                            .minDepth = 0.0f,
                            .maxDepth = 1.0f}},
             .scissors = {{.offset = {.x = 0, .y = 0},
                           .extent = swapchainData->swapchain->extent}},
             .required = true},
        .rasterizationState = {.depthClampEnable = VK_FALSE,
                               .rasterizerDiscardEnable = VK_FALSE,
                               .polygonMode = VK_POLYGON_MODE_FILL,
                               .cullMode = VK_CULL_MODE_BACK_BIT,
                               .frontFace = VK_FRONT_FACE_CLOCKWISE,
                               .depthBiasEnable = VK_FALSE,
                               .lineWidth = 1.0f,
                               .required = true},
        .multisampleState = {.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
                             .sampleShadingEnable = VK_FALSE,
                             .required = true},
        .colorBlendState = {.logicOpEnable = VK_FALSE,
                            .logicOp = VK_LOGIC_OP_COPY,
                            .attachments = {{.blendEnable = VK_FALSE,
                                             .colorWriteMask =
                                                 VK_COLOR_COMPONENT_R_BIT |
                                                 VK_COLOR_COMPONENT_G_BIT |
                                                 VK_COLOR_COMPONENT_B_BIT |
                                                 VK_COLOR_COMPONENT_A_BIT}},
                            .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f},
                            .required = true},
        .dynamicState = {.dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT,
                                           VK_DYNAMIC_STATE_SCISSOR},
                         .required = true},
        .layout = contextData.pipelineLayout->handle,
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