#pragma once
#include "VkBootstrap.h"
#include "VkBootstrapDispatch.h"
#include "app/ApplicationBase.h"
#include "app/RenderContext.h"
#include "core/Event.h"
#include "pv/PvCommandBuffers.h"
#include "pv/PvCommandPool.h"
#include "pv/PvFramebuffer.h"
#include "pv/PvPipelineLayout.h"
#include "pv/PvQueue.h"
#include "pv/PvRenderPass.h"
#include "pv/PvSurface.h"
#include <memory>
#include <optional>
#include <utility>
#include <vector>

namespace Pyra {

class RenderContextBase;
class PvImage;
class PvImageView;
class PvBootstrap;
class PvInstance;
class PvPhysicalDevice;
class PvSurface;
class PvDevice;
struct SwapchainData {
  std::vector<std::shared_ptr<PvImage>> images;
  std::vector<std::shared_ptr<PvImageView>> imageViews;
  vkb::Swapchain *swapchain;
  uint32_t currentIndex;
};

struct Queues {
  PvQueue graphics;
  PvQueue compute;
  PvQueue present;
  PvQueue transfer;
};

void updateSwapchain(void *, SwapchainData *);
class VulkanApplication : public ApplicationBase {

public:
  template <typename T> void addContext() {
    auto context = std::make_shared<T>();
    context->bootstrap = bootstrap;
    renderContexts.push_back(context);
  }

  template <typename T, typename... Args> void addContext(Args... args) {
    auto context = std::make_shared<T>(std::forward<Args...>(args...));
    renderContexts.push_back(context);
  }

  friend void updateSwapchain(void *, SwapchainData *);

protected:
  std::shared_ptr<PvBootstrap> bootstrap = std::make_shared<PvBootstrap>();
  SwapchainData swapchainData;
  Queues queues;
  std::shared_ptr<PvCommandPool> commandPool;
  std::shared_ptr<PvCommandBuffers> commandBuffers;
  std::vector<std::shared_ptr<RenderContextBase>> renderContexts;
  std::shared_ptr<PvRenderPass> renderPass;
  std::vector<std::shared_ptr<PvFramebuffer>> framebuffers;
  std::shared_ptr<PvPipelineLayout> pipelineLayout;
  std::shared_ptr<PvPipeline> pipeline;

  Event<SwapchainData *> swapchainUpdated;

  virtual void setUpBootstrap();

  virtual void recreateSwapchain();

  virtual void archiveSwapchainData();

  virtual void getQueue();

  virtual void createPipeline() = 0;

  virtual void createRenderPass() = 0;

  virtual void createCommandPool();

  virtual void createCommandBuffers();

  virtual void createSyncObject(){}

  virtual void createFramebuffers() = 0;

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
};

} // namespace Pyra