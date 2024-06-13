#pragma once
#include "VkBootstrapDispatch.h"
#include "app/ApplicationBase.h"
#include "pv/PvBootstrap.h"
#include "pv/PvDevice.h"
#include "pv/PvPhysicalDevice.h"
#include "pv/PvSurface.h"
#include "pv/pvInstance.h"
#include "window/WindowBase.h"
#include <memory>

namespace Pyra {

class VulkanApplication : public ApplicationBase {

protected:
  PvBootstrap bootstrap = PvBootstrap{};

  virtual void setUpBootstrap();

  std::shared_ptr<PvInstance> instance();
  std::shared_ptr<WindowBase> window();
  std::shared_ptr<PvPhysicalDevice> physicalDevice();
  std::shared_ptr<PvDevice> device();
  std::shared_ptr<PvSurface> surface();
  vkb::InstanceDispatchTable &vk();
  vkb::DispatchTable vkd();
};

} // namespace Pyra