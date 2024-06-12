#include "pv/PvBootstrap.h"
#include "VkBootstrap.h"
#include "core/logging.h"
#include "pv/PvDebugUtilsMessenger.h"
#include "pv/PvDevice.h"
#include "pv/PvPhysicalDevice.h"
#include "pv/PvSurface.h"
#include "pv/pvInstance.h"
#include "window/GlfwWindow.h"
#include <memory>
#include <volk.h>

namespace Pyra {

VkSurfaceKHR
PvBootstrap::defaultSurfaceConstruct(VkInstance instance,
                                     std::shared_ptr<WindowBase> window) {
  if (window != nullptr && instance != nullptr) {
    VkSurfaceKHR surface;
    VkSurfaceKHR *ptrSurface = &surface;
    if (window->createSurface(instance, ptrSurface)) {
      return surface;
    } else {
      ERROR("failed to create surface!");
    }
  } else
    ERROR("window or instance is not set up!");
  return nullptr;
}

PvBootstrap &
PvBootstrap::withInstanceBuilder(PvBootstrap::fp_instance_setting builder) {
  instance_setting = builder;
  return *this;
}

PvBootstrap &PvBootstrap::withPhysicalDeviceSelector(
    PvBootstrap::fp_physicalDevice_setting builder) {
  physicalDevice_setting = builder;
  return *this;
}

PvBootstrap &
PvBootstrap::withSurface(fp_surface_construct surface_constructor) {
  surface_construct = surface_constructor;
  return *this;
}

PvBootstrap &
PvBootstrap::withDeviceBuilder(PvBootstrap::fp_device_setting builder) {
  device_setting = builder;
  return *this;
}

PvBootstrap &
PvBootstrap::withSwapchainBuilder(PvBootstrap::fp_swapchain_setting &builder) {
  swapchain_setting = builder;
  return *this;
}

PvBootstrap &PvBootstrap::withWindow() { return withWindow<GlfwWindow>(); }

void PvBootstrap::build() {
  if (volkInitialize() == VK_SUCCESS) {
    if (init.window != nullptr)
      init.window->initWindow();
    if (instance_setting != nullptr) {
      vkb::InstanceBuilder instanceBuilder{};
      instance_setting(instanceBuilder);
      auto instance_rst = instanceBuilder.build();
      if (instance_rst) {
        auto instance = instance_rst.value();
        init.inst_disp = instance.make_table();
        init.instance = std::make_shared<PvInstance>(instance);
        if (instance.debug_messenger != nullptr) {
          init.messenger = std::make_shared<PvDebugUtilsMessenger>(instance);
        }
        volkLoadInstance(instance.instance);
        vkb::PhysicalDeviceSelector selector{instance};
        if (surface_construct != nullptr) {
          auto surface = surface_construct(instance.instance, init.window);
          init.surface = std::make_shared<PvSurface>(instance, surface);
          auto phyDevice_rst =
              selector.set_surface(init.surface->handle).select();
          if (phyDevice_rst) {
            init.physicalDevice =
                std::make_shared<PvPhysicalDevice>(phyDevice_rst.value());
            if (device_setting != nullptr) {
              vkb::DeviceBuilder deviceBuilder{phyDevice_rst.value()};
              device_setting(deviceBuilder);
              auto device_ret = deviceBuilder.build();
              if (device_ret) {
                auto device = device_ret.value();
                init.device = std::make_shared<PvDevice>(device);
                init.disp = device.make_table();
              } else {
                ERROR("device launch failed!");
              }
            } else {
              WARN("device is not set! No  device will be "
                   "built!");
            }
          } else {
            ERROR("No suitable physical device!");
          }
        } else {
          WARN(
              "Physical device selector is not set! No physical device will be "
              "selected!");
        }
      } else {
        ERROR("instance launch failed");
      }
    } else {
      WARN("instance is not set! No instance will be built!");
    }
  } else {
    ERROR("Failed to initialize volk!");
  }
}

} // namespace Pyra