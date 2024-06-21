#pragma once
#include "pv/PvCommon.h"
#include "pv/PvFramebuffer.h"
#include "pv/PvPipeline.h"
#include "pv/PvRenderPass.h"
#include <memory>
#include <vector>
namespace Pyra {

struct SwapchainData;

struct ContextData {
  std::vector<std::shared_ptr<PvFramebuffer>> framebuffers;
  std::shared_ptr<PvRenderPass> renderPass;
  std::shared_ptr<PvPipeline> pipeline;
};

class RenderContextBase {

  friend class VulkanApplication;
  friend void updateSwapchain(void *, SwapchainData *);

protected:
  std::shared_ptr<PvBootstrap>bootstrap;
  SwapchainData *swapchainData;

  virtual void createFramebuffers() {}

  virtual void createRenderPass() {}

  virtual void createPipeline() {}

  virtual void createCommandPool() {}

  virtual void createCommandBuffers() {}

  virtual void createSyncObjects() {}

private:
  void init();
};

template <typename ContextData>
class RenderContext : public RenderContextBase{
  protected:
  ContextData contextData;
};

} // namespace Pyra