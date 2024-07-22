#pragma once
#include "pv/PvCommon.h"
#define NOGDI
#include "PvResource.h"
#include "window/WindowBase.h"
#if WIN32
#include <vulkan/vulkan_win32.h>
#include <windows.h>
#endif

namespace Pyra {

class PvSurface;
struct PvDisplaySurfaceCreateInfoKHR;

template <> struct CreateInfo_T<PvSurface> {
  using type = PvDisplaySurfaceCreateInfoKHR;
};

struct PvDisplaySurfaceCreateInfoKHR
    : PvInfo<VkDisplaySurfaceCreateInfoKHR, PvSurface> {
  VkDisplaySurfaceCreateFlagsKHR flags;
  VkDisplayModeKHR displayMode;
  uint32_t planeIndex;
  uint32_t planeStackIndex;
  VkSurfaceTransformFlagBitsKHR transform;
  float globalAlpha;
  VkDisplayPlaneAlphaFlagBitsKHR alphaMode;
  VkExtent2D imageExtent;

  friend PvSurface;
  friend PvInfo<VkDisplaySurfaceCreateInfoKHR, PvSurface>;

private:
  void assign();
};

#if WIN32
struct PvWin32SurfaceCreateInfoKHR
    : PvInfo<VkWin32SurfaceCreateInfoKHR, PvSurface> {
  VkWin32SurfaceCreateFlagsKHR flags;
  HINSTANCE hinstance;
  HWND hwnd;

  friend PvSurface;
  friend PvInfo<VkWin32SurfaceCreateInfoKHR, PvSurface>;

private:
  void assign();
};
#endif
struct PvHeadlessSurfaceCreateInfoEXT
    : PvInfo<VkHeadlessSurfaceCreateInfoEXT, PvSurface> {
  VkHeadlessSurfaceCreateFlagsEXT flags;

  friend PvSurface;
  friend PvInfo<VkHeadlessSurfaceCreateInfoEXT, PvSurface>;

private:
  void assign();
};

struct PvSurfaceFromWindowInfo
    : PvInfo<VkDisplaySurfaceCreateInfoKHR, PvSurface> {
  WindowBase *window;
};

class PvSurface : public PvResource<VkSurfaceKHR, PFN_vkDestroySurfaceKHR> {

public:
  bool init(PvDisplaySurfaceCreateInfoKHR &info);

  template <typename... T>
  PvSurface(PvDisplaySurfaceCreateInfoKHR &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  };
  
#if WIN32
  bool init(PvWin32SurfaceCreateInfoKHR &info);

  template <typename... T>
  PvSurface(PvWin32SurfaceCreateInfoKHR &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  };
#endif

  bool init(PvHeadlessSurfaceCreateInfoEXT &info);

  template <typename... T>
  PvSurface(PvHeadlessSurfaceCreateInfoEXT &info, T... infos) {
    info.assign();
    (info + ... + infos);
    init(info);
  };

  PvSurface(PvSurfaceFromWindowInfo &info);

  PvSurface(PvTable *table, VkSurfaceKHR surface,
            ManageOperation op = AUTO_MANAGE);
};
} // namespace Pyra