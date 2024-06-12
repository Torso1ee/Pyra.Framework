#pragma once
#include "WindowBase.h"
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Pyra {

class GlfwWindow : public WindowBase {

public:
  ~GlfwWindow() override;

protected:
  void initWindow() override;

  bool shouldClose() override;

  void pollEvent() override;

  void waitEvent() override;

  bool createSurface(VkInstance instance, VkSurfaceKHR *surface,
                     VkAllocationCallbacks *callback = nullptr) override;

  std::vector<const char *> getRequiredExtensions() override;

  Extent getExtent() override;

private:
  void initGlfwWindow();
  GLFWwindow *window;
};
} // namespace Pyra