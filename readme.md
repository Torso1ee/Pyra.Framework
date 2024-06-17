# Pyra.Framework

Help to construct a vulkan application methodically. Developing...

## Feature

- [x] All vulkan resources are managed by library. Don't need to call vkdestroy.... like function and don't need to care about release of the vulkan object.

- [x] Simplify createInfo construct. Completely support Designated initializers.

  ```c++
  //VkCreateInfo
  typedef struct VkDeviceCreateInfo {
      VkStructureType                    sType;
      const void*                        pNext;
      VkDeviceCreateFlags                flags;
      uint32_t                           queueCreateInfoCount;
      const VkDeviceQueueCreateInfo*     pQueueCreateInfos;
      uint32_t                           enabledLayerCount;
      const char* const*                 ppEnabledLayerNames;
      uint32_t                           enabledExtensionCount;
      const char* const*                 ppEnabledExtensionNames;
      const VkPhysicalDeviceFeatures*    pEnabledFeatures;
  } VkDeviceCreateInfo;
  //PvCreateInfo for example
  struct PvDeviceCreateInfo {
    VkDeviceCreateFlags flags;
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::vector<const char *> enabledLayers;
    std::vector<const char *> enabledExtensions;
    VkPhysicalDeviceFeatures enabledFeatures;
  };
  ```

- [x] Integrate [VMA]([GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator: Easy to integrate Vulkan memory allocation library (github.com)](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator)) and [vk-bootstrap]([charles-lunarg/vk-bootstrap: Vulkan Bootstrapping Iibrary (github.com)](https://github.com/charles-lunarg/vk-bootstrap)). Configure your vulkan instance just as a web application.

  ```c++
  bootstrap
        .withInstanceBuilder([](auto builder) {
         ...
        })
        .withPhysicalDeviceSelector([](auto builder) {
  	   ...
        })
        .withDeviceBuilder([](auto builder) {
         ...
        })
        .withWindow()
        .withSurface()
        .withSwapchainBuilder([](auto builder) {
  		...
        })
        .build();
  ```

- [x] Simplify construction of “[Extending Structures](https://docs.vulkan.org/spec/latest/chapters/fundamentals.html#fundamentals-validusage-pNext)”.

  ```c++
  //example: create instance with messenger
  PvInstanceCreateInfo ins_info{...};
  PvDebugUtilsMessengerCreateInfo dbg_info{...};
  auto instance =  bootstrap.make<PvInstance>(ins_info, dbg_info);
  ```

## Todo

scene graph

more functions of  rendering 

add more vulkan example

...

