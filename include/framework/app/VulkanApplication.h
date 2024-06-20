#pragma once
#include "VkBootstrap.h"
#include "VkBootstrapDispatch.h"
#include "app/ApplicationBase.h"
#include "app/RenderContext.h"
#include "core/Event.h"
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
  std::optional<VkQueue> graphics;
  std::optional<VkQueue> compute;
  std::optional<VkQueue> present;
  std::optional<VkQueue> transfer;
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
  Event<SwapchainData *> swapchainUpdated;

  virtual void setUpBootstrap();

  virtual void recreateSwapchain();

  virtual void archiveSwapchainData();

  virtual void getQueue();

  virtual bool perFrame();

  void registerEvent() override;

  void internalRun() override;

  void preRun() override;

  std::vector<std::shared_ptr<RenderContextBase>> renderContexts;
  std::shared_ptr<PvInstance> instance();
  std::shared_ptr<WindowBase> window();
  std::shared_ptr<PvPhysicalDevice> physicalDevice();
  std::shared_ptr<PvDevice> device();
  std::shared_ptr<PvSurface> surface();
  vkb::InstanceDispatchTable &vk();
  vkb::DispatchTable vkd();
};

} // namespace Pyra