#include "pv/PvBootstrap.h"
#include "VkBootstrap.h"
#include "core/GlslCompiler.h"
#include "core/logging.h"
#include "pv/PvCommon.h"
#include "pv/PvDebugUtilsMessenger.h"
#include "pv/PvDevice.h"
#include "pv/PvPhysicalDevice.h"
#include "pv/PvQueue.h"
#include "pv/PvShaderModule.h"
#include "pv/PvSurface.h"
#include "pv/PvSwapchain.h"
#include "pv/PvInstance.h"
#include "window/GlfwWindow.h"
#include <memory>
#include <volk.h>

namespace Pyra
{

  VKAPI_ATTR VkBool32 VKAPI_CALL
  customDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                      VkDebugUtilsMessageTypeFlagsEXT messageType,
                      const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                      void *pUserData)
  {
    INFO("validation layer: {}", pCallbackData->pMessage);
    return VK_TRUE;
  }

  VkSurfaceKHR
  PvBootstrap::defaultSurfaceConstruct(VkInstance instance,
                                       std::shared_ptr<WindowBase> window)
  {
    if (window != nullptr && instance != nullptr)
    {
      VkSurfaceKHR surface;
      VkSurfaceKHR *ptrSurface = &surface;
      if (window->createSurface(instance, ptrSurface))
      {
        return surface;
      }
      else
      {
        ERROR("failed to create surface!");
      }
    }
    else
      ERROR("window or instance is not set up!");
    return nullptr;
  }

  PvBootstrap *
  PvBootstrap::withInstanceBuilder(PvBootstrap::fp_instance_setting builder)
  {
    instance_setting = builder;
    return this;
  }

  PvBootstrap *PvBootstrap::withPhysicalDeviceSelector(
      PvBootstrap::fp_physicalDevice_setting builder)
  {
    physicalDevice_setting = builder;
    return this;
  }

  PvBootstrap *
  PvBootstrap::withSurface(fp_surface_construct surface_constructor)
  {
    surface_construct = surface_constructor;
    return this;
  }

  PvBootstrap *
  PvBootstrap::withDeviceBuilder(PvBootstrap::fp_device_setting builder)
  {
    device_setting = builder;
    return this;
  }

  PvBootstrap *
  PvBootstrap::withSwapchainBuilder(PvBootstrap::fp_swapchain_setting builder)
  {
    swapchain_setting = builder;
    return this;
  }

  PvBootstrap *PvBootstrap::withVmaAllocator(fp_vmaAllocator_setting setting)
  {
    vmaAllocator_setting = setting;
    return this;
  }

  PvBootstrap *PvBootstrap::withWindow() { return withWindow<GlfwWindow>(); }

  void PvBootstrap::build(bool validation)
  {
    if (volkInitialize() == VK_SUCCESS)
    {
      if (init.window != nullptr)
        init.window->initWindow();
      vkb::InstanceBuilder instanceBuilder{};
      instanceBuilder.set_debug_callback(customDebugCallback);
      // Default setting
      if (validation)
      {
        instanceBuilder.set_debug_messenger_severity(
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
            .set_debug_messenger_type(
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
            .request_validation_layers();
      }
      if (instance_setting != nullptr)
        instance_setting(instanceBuilder);
      auto instance_rst = instanceBuilder.build();
      if (instance_rst)
      {
        table.instance = instance_rst.value();
        table.inst_disp = table.instance.make_table();
        init.instance = std::make_shared<PvInstance>(&table);
        if (table.instance.debug_messenger != nullptr)
        {
          init.messenger = std::make_shared<PvDebugUtilsMessenger>(&table);
        }
        volkLoadInstance(table.instance.instance);
        vkb::PhysicalDeviceSelector selector{table.instance};
        selector.add_required_extensions({"VK_EXT_graphics_pipeline_library", "VK_KHR_pipeline_library"});
        if (physicalDevice_setting != nullptr)
          physicalDevice_setting(selector);
        if (surface_construct != nullptr)
        {
          auto surface = surface_construct(table.instance, init.window);
          init.surface = std::make_shared<PvSurface>(&table, surface);
          table.physicalDevice =
              selector.set_surface(init.surface->handle).select().value();
          if (table.physicalDevice)
          {
            init.physicalDevice = std::make_shared<PvPhysicalDevice>(&table);

            vkb::DeviceBuilder deviceBuilder{table.physicalDevice};
            if (device_setting != nullptr)
              device_setting(deviceBuilder);
            auto device_ret = deviceBuilder.build();
            if (device_ret)
            {
              table.device = device_ret.value();
              table.disp = table.device.make_table();
              init.device = std::make_shared<PvDevice>(&table);

              VmaVulkanFunctions vulkanFunctions = {
                  .vkGetInstanceProcAddr =
                      table.instance.fp_vkGetInstanceProcAddr,
                  .vkGetDeviceProcAddr = table.device.fp_vkGetDeviceProcAddr,
              };
              VmaAllocatorCreateInfo info{
                  .flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT,
                  .physicalDevice = init.physicalDevice->handle,
                  .device = init.device->handle,
                  .pVulkanFunctions = &vulkanFunctions,
                  .instance = init.instance->handle,
                  .vulkanApiVersion = VK_API_VERSION_1_0,
              };

              if (vmaAllocator_setting != nullptr)
                vmaAllocator_setting(&table, info);
              if (vmaCreateAllocator(&info, &table.allocator) != VK_SUCCESS)
              {
                ERROR("allocator creation failed!");
              }

              if (swapchain_setting != nullptr)
              {
                createSwapchain();
              }
            }
            else
            {
              ERROR("device launch failed!");
            }
          }
          else
          {
            ERROR("No suitable physical device!");
          }
        }
        else
        {
          WARN(
              "Surface constructor is not set!");
        }
      }
      else
      {
        ERROR("instance launch failed");
      }
    }
    else
    {
      ERROR("Failed to initialize volk!");
    }
  }

  bool PvBootstrap::createSwapchain()
  {
    vkb::SwapchainBuilder swapchain_builder{table.device};
    swapchain_setting(swapchain_builder);
    auto swap_ret = swapchain_builder.set_old_swapchain(table.swapchain).build();
    if (!swap_ret)
    {
      ERROR(swap_ret.error().message());
      return false;
    }
    table.swapchain = swap_ret.value();
    init.swapchain = std::make_shared<PvSwapchain>(&table);
    return true;
  }

  std::shared_ptr<PvShaderModule>
  PvBootstrap::createShaderModule(const char *code, shaderc_shader_kind kind)
  {
    CreateInfo<PvShaderModule> info{};
    GlslCompiler::Compile(code, kind, info.codes);
    return make<PvShaderModule>(info);
  }

  std::shared_ptr<PvQueue> PvBootstrap::getQueue(vkb::QueueType type)
  {
    if (table.device.device != nullptr && table.device.surface != nullptr)
    {
      auto gq = table.device.get_queue(type);
      INFO("get {} queue", typeid((type)).name());
      if (!gq.has_value())
      {
        ERROR("failed to get queue");
        throw std::runtime_error("failed to get queue");
      }
      return std::make_shared<PvQueue>(
          &table, table.device.get_queue_index(type).value(), gq.value());
    }
    else
    {
      // TODO
      throw;
    }
  }

} // namespace Pyra