#include "pv/PvResource.h"
#include "vulkan/vulkan_core.h"

namespace Pyra {

std::shared_ptr<HandleManager> HandleManager::get() {
  if (handleManager == nullptr)
    handleManager = std::make_shared<HandleManager>();
  return handleManager;
}

std::shared_ptr<HandleManager> HandleManager::handleManager = nullptr;

int HandleManager::count = 0;

void HandleManager::HandleManager::release() {
  //   VulkanResource<VkSampler, PFN_vkDestroySampler>::releaseAll();
  //   VulkanResource<VkBuffer, PFN_vkDestroyBuffer>::releaseAll();
  //   VulkanResource<VkCommandBuffer *,
  //   PFN_vkFreeCommandBuffers>::releaseAll(); VulkanResource<VkPipeline,
  //   PFN_vkDestroyPipeline>::releaseAll();
  PvResource<VkRenderPass, PFN_vkDestroyRenderPass>::releaseAll();
  // VulkanResource<VkPipelineLayout,
  //    PFN_vkDestroyPipelineLayout>::releaseAll();
  //    VulkanResource<VkShaderModule, PFN_vkDestroyShaderModule>::releaseAll();
  PvResource<VkFramebuffer, PFN_vkDestroyFramebuffer>::releaseAll();
  //   VulkanResource<VkSampler, PFN_vkDestroySampler>::releaseAll();
  PvResource<VkImageView, PFN_vkDestroyImageView>::releaseAll();
  //   VulkanResource<VkImage, PFN_vkDestroyImage>::releaseAll();
  PvResource<VkDeviceMemory, PFN_vkFreeMemory>::releaseAll();
  //   VulkanResource<VkPipelineCache,
  //   PFN_vkDestroyPipelineCache>::releaseAll(); VulkanResource<VkCommandPool,
  //   PFN_vkDestroyCommandPool>::releaseAll(); VulkanResource<VkSemaphore,
  //   PFN_vkDestroySemaphore>::releaseAll(); VulkanResource<VkFence,
  //   PFN_vkDestroyFence>::releaseAll();
  PvResource<VkSwapchainKHR, PFN_vkDestroySwapchainKHR>::releaseAll();
  //   VulkanResource<VkDescriptorPool,
  //   PFN_vkDestroyDescriptorPool>::releaseAll();
  //   VulkanResource<VkDescriptorSetLayout,
  //                  PFN_vkDestroyDescriptorSetLayout>::releaseAll();
  PvResource<VkSurfaceKHR, PFN_vkDestroySurfaceKHR>::releaseAll();
  PvResource<VkDevice, PFN_vkDestroyDevice>::releaseAll();
  PvResource<VkDebugUtilsMessengerEXT,
             PFN_vkDestroyDebugUtilsMessengerEXT>::releaseAll();
  PvResource<VkInstance, PFN_vkDestroyInstance>::releaseAll();
}

HandleManager::~HandleManager() { release(); }

} // namespace Pyra