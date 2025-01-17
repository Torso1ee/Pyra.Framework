#pragma once
#include "pv/PvBufferVma.h"
#include <VkBootstrap.h>
#include <memory>
#include <optional>
#include <pv/PvCommon.h>
#include <shaderc/shaderc.h>
#include <vk_mem_alloc.h>
#include <volk.h>

namespace Pyra {

class PvInstance;
class PvPhysicalDevice;
class PvSurface;
class PvDevice;
class PvSwapchain;
class PvDebugUtilsMessenger;
class WindowBase;
class PvShaderModule;
class PvQueue;

struct PbBootstrapCreateInfo {
  bool needSurface;
  bool needWindow;
  bool needSwapchain;
};

struct PvTable {
  vkb::InstanceDispatchTable inst_disp;
  vkb::DispatchTable disp;
  vkb::Instance instance;
  vkb::Device device;
  vkb::PhysicalDevice physicalDevice;
  vkb::Swapchain swapchain;
  VmaAllocator allocator;
};

class VulkanApplication;
class PvBootstrap {

public:
  struct Init {
    std::shared_ptr<WindowBase> window;
    std::shared_ptr<PvInstance> instance;
    std::shared_ptr<PvPhysicalDevice> physicalDevice;
    std::shared_ptr<PvSurface> surface;
    std::shared_ptr<PvDevice> device;
    std::shared_ptr<PvDebugUtilsMessenger> messenger;
    std::shared_ptr<PvSwapchain> swapchain;
  } init;

  using fp_surface_construct = VkSurfaceKHR (*)(VkInstance,
                                                std::shared_ptr<WindowBase>);

  using fp_vmaAllocator_setting = void (*)(PvTable *table,
                                           VmaAllocatorCreateInfo &info);

  using fp_instance_setting = void (*)(vkb::InstanceBuilder &builder);

  using fp_device_setting = void (*)(vkb::DeviceBuilder &builder);

  using fp_physicalDevice_setting =
      void (*)(vkb::PhysicalDeviceSelector &builder);

  using fp_swapchain_setting = void (*)(vkb::SwapchainBuilder &builder);

  static VkSurfaceKHR defaultSurfaceConstruct(VkInstance,
                                              std::shared_ptr<WindowBase>);

  PvBootstrap *withInstanceBuilder(fp_instance_setting setting);

  PvBootstrap *withPhysicalDeviceSelector(fp_physicalDevice_setting setting);

  PvBootstrap *withSurface(
      fp_surface_construct surface_constructor = defaultSurfaceConstruct);

  PvBootstrap *withDeviceBuilder(fp_device_setting setting);

  PvBootstrap *withSwapchainBuilder(fp_swapchain_setting builder);

  PvBootstrap *withVmaAllocator(fp_vmaAllocator_setting setting);

  std::shared_ptr<PvQueue> getQueue(vkb::QueueType);

  template <typename Window> PvBootstrap *withWindow() {
    init.window = std::make_shared<Window>();
    return this;
  }

  PvBootstrap *withWindow();

  void InitVmaAllocationCreateInfo(VmaAllocatorCreateInfo &info);

  void build(bool validation = true);

  template <typename T, typename... T1>
  std::shared_ptr<T> make(CreateInfo<T> info, T1... args) {
    info.table = &table;
    return std::make_shared<T>(info, args...);
  }

  template <typename T> std::shared_ptr<T> make(T info) {
    info.table = &table;
    return std::make_shared<T>(info);
  }

  std::shared_ptr<PvShaderModule> createShaderModule(const char *code,
                                                     shaderc_shader_kind kind);

  template <typename T>
  std::shared_ptr<PvBufferVma> createStagingBuffer(const std::vector<T> &data) {
    return createStagingBuffer(data.size() * sizeof(T), data.data());
  }

  template <typename T>
  std::shared_ptr<PvBufferVma> createStagingBuffer(const T &data) {
    return createStagingBuffer(sizeof(T), &data);
  }

  std::shared_ptr<PvBufferVma> createStagingBuffer(VkDeviceSize size,
                                                   const void *data) {
    CreateInfo<PvBufferVma> info{
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .allocInfo = {
            .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT |
                     VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT}};
    auto stage_buffer = make<PvBufferVma>(info);
    stage_buffer->update(reinterpret_cast<const uint8_t *>(data), size);
    return stage_buffer;
  }

  friend VulkanApplication;

private:
  fp_surface_construct surface_construct = nullptr;

  fp_instance_setting instance_setting = nullptr;

  fp_device_setting device_setting = nullptr;

  fp_physicalDevice_setting physicalDevice_setting = nullptr;

  fp_swapchain_setting swapchain_setting = nullptr;

  fp_vmaAllocator_setting vmaAllocator_setting = nullptr;

  bool createSwapchain();

  bool isExtensionSupported(const char *ext);

  PvTable table;
};

} // namespace Pyra