#pragma once
#include "pv/PvCommon.h"
#include "pv/PvFence.h"
#include "pv/PvFramebuffer.h"
#include "pv/PvImage.h"
#include "pv/PvImageView.h"
#include "pv/PvSemaphore.h"
#include <cstdint>
#include <memory>
#include <vector>

namespace Pyra {

struct SwapchainData;

class RenderContext {
public:
  std::vector<std::shared_ptr<PvSemaphore>> availableSemaphores;
  std::vector<std::shared_ptr<PvSemaphore>> finishedSemaphores;
  std::vector<std::shared_ptr<PvFence>> inFlightFences;
  std::vector<std::shared_ptr<PvFence>> imageInFlight;

  uint32_t activeFrame;

  PvBootstrap *bootstrap;

  virtual void createSyncObjects();

  virtual void init(PvBootstrap *boot);

  virtual bool nextFrame();
};

} // namespace Pyra