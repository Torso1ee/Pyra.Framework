#pragma once
#include "pv/PvDebugUtilsMessenger.h"
#include "pv/PvDevice.h"
#include "pv/PvPhysicalDevice.h"
#include "pv/PvSurface.h"
#include "pv/pvInstance.h"
#include "window/WindowBase.h"
#include <VkBootstrap.h>
#include <memory>
#include <pv/PvCommon.h>
#include <volk.h>


namespace Pyra {

struct PbBootstrapCreateInfo {
  bool needSurface;
  bool needWindow;
  bool needSwapchain;
};

class PvBootstrap {

public:
  struct Init {
    std::shared_ptr<WindowBase> window;
    std::shared_ptr<PvInstance> instance;
    vkb::InstanceDispatchTable inst_disp;
    std::shared_ptr<PvPhysicalDevice> physicalDevice;
    std::shared_ptr<PvSurface> surface;
    std::shared_ptr<PvDevice> device;
    std::shared_ptr<PvDebugUtilsMessenger> messenger;
    vkb::DispatchTable disp;
    vkb::Swapchain swapchain;
  } init;

  using fp_surface_construct = VkSurfaceKHR (*)(VkInstance,
                                                std::shared_ptr<WindowBase>);

  using fp_instance_setting = void (*)(vkb::InstanceBuilder &builder);

  using fp_device_setting = void (*)(vkb::DeviceBuilder &builder);

  using fp_physicalDevice_setting =
      void (*)(vkb::PhysicalDeviceSelector &builder);

  using fp_swapchain_setting = void (*)(vkb::SwapchainBuilder &builder);

  static VkSurfaceKHR defaultSurfaceConstruct(VkInstance,
                                              std::shared_ptr<WindowBase>);

  PvBootstrap &withInstanceBuilder(fp_instance_setting setting);

  PvBootstrap &withPhysicalDeviceSelector(fp_physicalDevice_setting setting);

  PvBootstrap &withSurface(
      fp_surface_construct surface_constructor = defaultSurfaceConstruct);

  PvBootstrap &withDeviceBuilder(fp_device_setting setting);

  PvBootstrap &withSwapchainBuilder(fp_swapchain_setting &builder);

  template <typename Window> PvBootstrap &withWindow() {
    init.window = std::make_shared<Window>();
    return *this;
  }

  PvBootstrap &withWindow();

  void build();

  template <typename T, typename... T1>
  std::shared_ptr<T> make(CreateInfo<T> info, T1... args) {
    return std::make_shared<typename std::shared_ptr<T>>(info, args...);
  }

private:
  fp_surface_construct surface_construct = nullptr;

  fp_instance_setting instance_setting = nullptr;

  fp_device_setting device_setting = nullptr;

  fp_physicalDevice_setting physicalDevice_setting = nullptr;

  fp_swapchain_setting swapchain_setting = nullptr;
};

} // namespace Pyra