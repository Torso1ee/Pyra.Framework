#pragma once
#include "RenderContext.h"
#include "VkBootstrapDispatch.h"
#include "app/ApplicationBase.h"
#include "pv/PvBootstrap.h"
#include "pv/PvDevice.h"
#include "pv/PvFramebuffer.h"
#include "pv/PvPhysicalDevice.h"
#include "pv/PvSurface.h"
#include "pv/pvInstance.h"
#include "window/WindowBase.h"
#include <memory>
#include <vector>

namespace Pyra {

struct SwapchainData {
  std::shared_ptr<PvImage> image;
  std::shared_ptr<PvImageView> imageView;
  std::shared_ptr<PvFramebuffer> framebuffer;
};

class VulkanApplication : public ApplicationBase {

protected:
  PvBootstrap bootstrap = PvBootstrap{};
  std::vector<SwapchainData> swapchainDatas;
  uint32_t activeIndex;

  virtual void setUpBootstrap();

  virtual void recreateSwapchain();

  virtual void archiveSwapchainData();

  virtual void createFramebuffer(SwapchainData& data);

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