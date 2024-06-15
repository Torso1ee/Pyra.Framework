#include <cstdint>
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

} // namespace Pyra