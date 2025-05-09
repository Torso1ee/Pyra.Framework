#include "app/App.h"
#include "pv/Pv.h"
#include <cstdint>
#include <memory>

using namespace Pyra;

class Triangle : public VulkanApplication {
  void createRenderPass() override {
    CreateInfo<PvRenderPass> info{
        .attachments = {{.format = swapchain()->imageFormat,
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
    framebuffers.resize(swapchain()->images.size());
    for (uint32_t i = 0; i < swapchain()->images.size(); i++) {
      CreateInfo<PvFramebuffer> info{
          .renderPass = renderPass->handle,
          .attachments = {swapchain()->imageViews[i]->handle},
          .width = swapchain()->extent.width,
          .height = swapchain()->extent.height,
          .layers = 1};
      framebuffers[i] = bootstrap->make<PvFramebuffer>(info);
    }
  }

  void createPipeline() override {
    CreateInfo<PvPipelineLayout> plInfo;
    pipelineLayout = bootstrap->make<PvPipelineLayout>(plInfo);
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
                            .width = (float)swapchain()->extent.width,
                            .height = (float)swapchain()->extent.height,
                            .minDepth = 0.0f,
                            .maxDepth = 1.0f}},
             .scissors = {{.offset = {.x = 0, .y = 0},
                           .extent = swapchain()->extent}},
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
        .layout = pipelineLayout->handle,
        .renderPass = renderPass->handle,
        .subpass = 0,
        .basePipelineHandle = nullptr};
    pipeline = bootstrap->make<PvPipeline>(info);
  }

  void recordCommandBuffer(uint32_t imageIndex) override {
    auto cmd = commandBuffers->get(activeFrame);
    cmd.reset()
        .beginCommandBuffer({})
        .setViewport(
            {.firstViewport = 0,
             .viewports = {{.x = 0.0f,
                            .y = 0.0f,
                            .width = (float)swapchain()->extent.width,
                            .height = (float)swapchain()->extent.height,
                            .minDepth = 0.0f,
                            .maxDepth = 0.0f}}})
        .setScissor(
            {.firstScissor = 0,
             .scissors = {{.offset = {0, 0}, .extent = swapchain()->extent}}})
        .beginRenderPass(
            {.renderPass = renderPass->handle,
             .framebuffer = framebuffers[imageIndex]->handle,
             .renderArea = {.offset = {0, 0}, .extent = swapchain()->extent},
             .clearValues = {{{0.0f, 0.0f, 0.0f, 1.0f}}},
             .subpassContents = VK_SUBPASS_CONTENTS_INLINE})
        .bindPipeline({.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                       .pipeline = pipeline->handle})
        .draw({.vertexCount = 3,
               .instanceCount = 1,
               .firstVertex = 0,
               .firstInstance = 0})
        .endRenderPass()
        .endCommandBuffer();
  }
};

int main(int, char **) {
  Triangle app;
  app.run();
  return 0;
}