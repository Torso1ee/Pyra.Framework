#include "app/App.h"
#include "pv/Pv.h"
#include "scene/Scene.h"
#include <memory>

using namespace Pyra;

struct Model {
  std::unique_ptr<PvMesh> mesh;
  std::shared_ptr<PvPipeline> pipeline;
  PvTexture texture;
};

class Instancing : public VulkanApplication {

  std::shared_ptr<PvCamera> camera;

  void prepareData() override {
    initializeCamera();
    setUpData();
  }

  void setUpData() {}

  void initializeCamera() {
    camera = std::make_shared<PvCamera>();
    // TODO：camera set
  }

  void createPipeline() override {
    createDescriptorSetLayout();
    createDescriptorPool();
  }

  void prepareInstanceData() {}

  void prepareUniformBuffers() {}

  void createDescriptorSetLayout() {}

  void createDescriptorPool() {}

  void setUpBootstrap() override {

    bootstrap->withPhysicalDeviceSelector([](auto builder) {
      VkPhysicalDeviceFeatures features{
          .samplerAnisotropy = VK_TRUE,
          .textureCompressionETC2 = VK_TRUE,
          .textureCompressionASTC_LDR = VK_TRUE,
          .textureCompressionBC = VK_TRUE,
      };
      builder.set_required_features(features);
    });
  }
};
int main(int, char **) {
  // Instancing app;
  // app.run();
  // return 0;
}