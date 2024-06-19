#include "pv/PvCommon.h"
#include "pv/PvFramebuffer.h"
#include "pv/PvRenderPass.h"
#include <memory>
#include <vector>
namespace Pyra {

struct SwapchainData;
struct ContextData {
  std::vector<std::shared_ptr<PvFramebuffer>> framebuffers;
  std::shared_ptr<PvRenderPass> renderPass;
};
class RenderContext {

  ContextData contextData;
  SwapchainData *swapChainData;

  friend class VulkanApplication;

protected:
  PvBootstrap *bootstrap;

  virtual void createFramebuffers(SwapchainData &data);

private:
  void init(PvTable *) {}
};

} // namespace Pyra