#pragma once
#include "RenderContext.h"
#include "VkBootstrapDispatch.h"
#include "app/ApplicationBase.h"
#include "pv/PvBootstrap.h"
#include "pv/PvDevice.h"
#include "pv/PvPhysicalDevice.h"
#include "pv/PvSurface.h"
#include "pv/pvInstance.h"
#include "window/WindowBase.h"
#include <memory>
#include <vector>


namespace Pyra {

class VulkanApplication : public ApplicationBase {

protected:
  PvBootstrap bootstrap = PvBootstrap{};
  std::vector<std::shared_ptr<RenderContext>> renderContexts;

  template <typename Context, typename... Contexts> void addContext() {
    auto context = std::make_shared<Context>();
    renderContexts.push_back(context);
    addContext<Contexts...>();
  }

  virtual void setUpBootstrap();

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