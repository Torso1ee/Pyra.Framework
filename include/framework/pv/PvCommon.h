#pragma once
#define NOGDI
#include <optional>
#include <set>
#include <volk.h>
namespace Pyra {

#define NULLPTR_IF_EMPTY(vector) vector.size() > 0 ? vector.data() : nullptr
template <typename T> struct CreateInfo_T {
  using type = T;
};

struct PvPlaceHolder {};

template <typename T> using CreateInfo = CreateInfo_T<T>::type;

struct QueueFamily {
  VkQueueFamilyProperties properties;
  uint32_t index;
  bool graphicsSupport;
  bool computeSupport;
  bool transferSupport;
  bool isPresentSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    VkBool32 pSupport = false;
    // vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface, &pSupport);
    return graphicsSupport && pSupport;
  }
  QueueFamily(VkQueueFamilyProperties prop, uint32_t i)
      : properties(prop), index(i) {
    graphicsSupport = prop.queueFlags & VK_QUEUE_GRAPHICS_BIT;
    computeSupport = prop.queueFlags & VK_QUEUE_COMPUTE_BIT;
    transferSupport = prop.queueFlags & VK_QUEUE_TRANSFER_BIT;
  }
};

struct QueueFamilyIndices {
  std::optional<uint32_t> graphics;
  std::optional<uint32_t> compute;
  std::optional<uint32_t> transfer;
  std::optional<uint32_t> present;
  bool isComplete() {
    return isPresentSupport() && transfer.has_value() && compute.has_value();
  }
  bool isPresentSupport() {
    return graphics.has_value() && present.has_value();
  }
  std::set<uint32_t> getCompleteIndices() {
    std::set<uint32_t> indices;
    if (graphics.has_value())
      indices.insert(graphics.value());
    if (compute.has_value())
      indices.insert(graphics.value());
    if (transfer.has_value())
      indices.insert(graphics.value());
    if (present.has_value())
      indices.insert(graphics.value());
    return indices;
  }
};

enum ManageOperation {
  AUTO_MANAGE,
  DO_NOT_MANAGE,
};

class PvBootstrap;
class PvTable;
template <typename T, typename Derived> struct PvInfo {
  ManageOperation operation = AUTO_MANAGE;
  VkAllocationCallbacks *callback = nullptr;

  friend Derived;
  template <typename, typename> friend class PvInfo;
  friend PvBootstrap;

public:
  void setTable(PvTable *t) { table = t; }

protected:
  T info;

private:
  PvTable *table;

  void init() {
    auto p = static_cast<CreateInfo<Derived> *>(this);
    p->assign();
  }

  template <typename TR> TR &operator+(TR &rInfo) {
    rInfo.init();
    info.pNext = &rInfo.info;
    return rInfo;
  }
};

template <typename T, typename... Args> struct MixinInfo;

template <typename T> struct MixinInfo<T> {
  struct Info : T {
    bool required = false;
  };
  Info info;
};

template <typename T, typename... Args> struct MixinInfo : MixinInfo<Args...> {
  struct Info : T {
    bool required = false;
  };
  Info info;
};

template <typename T> struct MemberInfo : T {
  bool required = false;
};

} // namespace Pyra