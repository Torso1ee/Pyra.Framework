#pragma once
#include "pv/Pv.h"
#include "VkBootstrapDispatch.h"
#include "app/ApplicationBase.h"
#include "app/RenderContext.h"
#include <memory>
#include <vector>

namespace Pyra {

class PvImage;
class PvImageView;
class PvBootstrap;
class PvInstance;
class PvPhysicalDevice;
class PvSurface;
class PvDevice;
struct Queues {
  PvQueue graphics;
  PvQueue compute;
  PvQueue present;
  PvQueue transfer;
};

void updateSwapchain(void *, SwapchainData *);
class VulkanApplication : public ApplicationBase {

public:

  friend void updateSwapchain(void *, SwapchainData *);

protected:
  std::shared_ptr<PvBootstrap> bootstrap = std::make_shared<PvBootstrap>();
  Queues queues;
  std::shared_ptr<PvCommandPool> commandPool;
  std::shared_ptr<PvCommandBuffers> commandBuffers;
  std::shared_ptr<PvRenderPass> renderPass;
  std::vector<std::shared_ptr<PvFramebuffer>> framebuffers;
  std::shared_ptr<PvPipelineLayout> pipelineLayout;
  std::shared_ptr<PvPipeline> pipeline;

  virtual void setUpBootstrap();

  virtual void recreateSwapchain();

  virtual void getQueue();

  virtual void createPipeline() = 0;

  virtual void createRenderPass() = 0;

  virtual void createCommandPool();

  virtual void createCommandBuffers();

  virtual void createSyncObject() {}

  virtual void createFramebuffers() = 0;

  virtual void createSyncObjects() = 0;

  virtual bool perFrame();

  void registerEvent() override;

  void internalRun() override;

  void preRun() override;

  std::shared_ptr<PvInstance> instance();
  std::shared_ptr<WindowBase> window();
  std::shared_ptr<PvPhysicalDevice> physicalDevice();
  std::shared_ptr<PvDevice> device();
  std::shared_ptr<PvSurface> surface();
  vkb::InstanceDispatchTable &vk();
  vkb::DispatchTable vkd();
  std::shared_ptr<PvSwapchain> swapchain();
};

} // namespace Pyra