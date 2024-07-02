#include "window/GlfwWindow.h"
#include "GLFW/glfw3.h"
#include "core/logging.h"
#include <volk.h>
#include "window/WindowBase.h"
#include <cstdint>


namespace Pyra {

	static void framebufferResizeCallback(GLFWwindow* window, int width,
		int height) {
		auto wnd = reinterpret_cast<WindowBase*>(glfwGetWindowUserPointer(window));
		wnd->resized.invoke({ .width = (uint32_t)width, .height = (uint32_t)height });
	}

	static void mouseButtonCallback(GLFWwindow* window, int button, int action,
		int mods) {
		auto wnd = reinterpret_cast<WindowBase*>(glfwGetWindowUserPointer(window));
		MouseEventArgs args{
			.mousePos = wnd->curPos,
			.diff = {0, 0},
			.scrollOffset = {0, 0},
			.leftButton = glfwGetMouseButton(window, 0) == 0 ? Released : Pressed,
			.middleButton = glfwGetMouseButton(window, 2) == 0 ? Released : Pressed,
			.rightButton = glfwGetMouseButton(window, 1) == 0 ? Released : Pressed,
		};
		if (action == 1)
			wnd->mouseDown.invoke(args);
		else
			wnd->mouseUp.invoke(args);
	}

	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
		auto wnd = reinterpret_cast<WindowBase*>(glfwGetWindowUserPointer(window));
		wnd->mouseMove.invoke({
			.mousePos = {xpos, ypos},
			.diff = {xpos - wnd->curPos.xPos, ypos - wnd->curPos.yPos},
			.scrollOffset = {0, 0},
			.leftButton = glfwGetMouseButton(window, 0) == 0 ? Released : Pressed,
			.middleButton = glfwGetMouseButton(window, 2) == 0 ? Released : Pressed,
			.rightButton = glfwGetMouseButton(window, 1) == 0 ? Released : Pressed,
			});
		wnd->curPos = { xpos, ypos };
	}

	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		auto wnd = reinterpret_cast<WindowBase*>(glfwGetWindowUserPointer(window));
		wnd->mouseScroll.invoke({
			.mousePos = wnd->curPos,
			.diff = {0, 0},
			.scrollOffset = {xoffset, yoffset},
			.leftButton = glfwGetMouseButton(window, 0) == 0 ? Released : Pressed,
			.middleButton = glfwGetMouseButton(window, 2) == 0 ? Released : Pressed,
			.rightButton = glfwGetMouseButton(window, 1) == 0 ? Released : Pressed,
			});
	}

	void GlfwWindow::initWindow() { initGlfwWindow(); }

	GlfwWindow::~GlfwWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	bool GlfwWindow::shouldClose() { return glfwWindowShouldClose(window); }

	void GlfwWindow::pollEvent() { glfwPollEvents(); }

	void GlfwWindow::waitEvent() { glfwWaitEvents(); }

	void GlfwWindow::initGlfwWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		window = glfwCreateWindow(800, 600, "Pyra", nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetCursorPosCallback(window, cursorPosCallback);
		glfwSetScrollCallback(window, scrollCallback);
	}

	bool GlfwWindow::createSurface(VkInstance instance, VkSurfaceKHR* surface,
		VkAllocationCallbacks* callback) {
		if (glfwCreateWindowSurface(instance, window, callback, surface) !=
			VK_SUCCESS) {
			ERROR("failed to create windowsurface!");
			return false;
		}
		return true;
	}

	Extent GlfwWindow::getExtent() {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
	}

	std::vector<const char*> GlfwWindow::getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> extensions(glfwExtensions,
			glfwExtensions + glfwExtensionCount);
		return extensions;
	}

} // namespace Pyra