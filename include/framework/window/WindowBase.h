#pragma once
#define NOMINMAX
#include "core/Event.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>
namespace Pyra {

	struct Extent {
		uint32_t width;
		uint32_t height;
	};

	struct ResizedEventArgs {
		uint32_t width;
		uint32_t height;
	};

	enum MouseState { Pressed, Released };

	struct MousePos {
		double xPos;
		double yPos;
	};

	struct MouseEventArgs {
		MousePos mousePos;
		MousePos diff;
		MousePos scrollOffset;
		MouseState leftButton;
		MouseState middleButton;
		MouseState rightButton;
	};

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

		virtual std::vector<const char*> getRequiredExtensions() = 0;

		virtual bool createSurface(VkInstance instance, VkSurfaceKHR* surface,
			VkAllocationCallbacks* callback = nullptr) = 0;

		virtual Extent getExtent() = 0;
	};
} // namespace Pyra