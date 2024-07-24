#pragma once
#include "app/ApplicationBase.h"
#include "pv/Pv.h"
#include <cstdint>
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
  std::shared_ptr<PvQueue> graphics;
  std::shared_ptr<PvQueue> compute;
  std::shared_ptr<PvQueue> present;
  std::shared_ptr<PvQueue> transfer;
};

class VulkanApplication : public ApplicationBase {

public:
  bool framebufferResized = false;

protected:
  struct Setting {
    uint32_t maxFramesInFlight = 2;
    const char *applicationName = "Pyra";
  } setting;

  std::shared_ptr<PvBootstrap> bootstrap = std::make_shared<PvBootstrap>();
  Queues queues;
  std::shared_ptr<PvCommandPool> commandPool;
  std::shared_ptr<PvCommandBuffers> commandBuffers;
  std::shared_ptr<PvRenderPass> renderPass;
  std::vector<std::shared_ptr<PvFramebuffer>> framebuffers;
  std::shared_ptr<PvPipelineLayout> pipelineLayout;
  std::shared_ptr<PvPipeline> pipeline;
  std::vector<std::shared_ptr<PvSemaphore>> availableSemaphores;
  std::vector<std::shared_ptr<PvSemaphore>> finishedSemaphores;
  std::vector<std::shared_ptr<PvFence>> inFlightFences;
  uint32_t activeFrame = 0;

  virtual void createSyncObjects();

  virtual void setUpBootstrap();

  virtual void configure();

  virtual void prepareData(){}

  virtual void recreateSwapchain();

  virtual void getQueue();

  virtual void createPipeline() = 0;

  virtual void createRenderPass() = 0;

  virtual void createCommandPool();

  virtual void createCommandBuffers();

  virtual void recordCommandBuffer(uint32_t imageIndex) = 0;

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
  std::shared_ptr<PvSwapchain> swapchain();
};

} // namespace Pyra