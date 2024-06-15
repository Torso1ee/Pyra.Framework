#pragma once
#define NOMINMAX
#include "core/Event.h"
#include "core/controlStruct.h"
#include <volk.h>
namespace Pyra {

class WindowBase {
public:
  Event<ResizedEventArgs> resized;
  Event<MouseEventArgs> mouseMove;
  Event<MouseEventArgs> mouseDown;
  Event<MouseEventArgs> mouseUp;
  Event<MouseEventArgs> mouseEnter;
  Event<MouseEventArgs> mouseLeave;
  Event<MouseEventArgs> mouseScroll;

  MousePos curPos;

  virtual ~WindowBase();
  virtual void initWindow() = 0;

  virtual bool shouldClose() = 0;

  virtual void pollEvent() = 0;

  virtual void waitEvent() = 0;

  virtual std::vector<const char *> getRequiredExtensions() = 0;

  virtual bool createSurface(VkInstance instance, VkSurfaceKHR *surface,
                             VkAllocationCallbacks *callback = nullptr) = 0;

  virtual Extent getExtent() = 0;
};
} // namespace Pyra