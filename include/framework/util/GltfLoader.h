#include "MeshLoader.h"
#include "scene/PvMesh.h"
#include <memory>
#include <tiny_gltf.h>

namespace Pyra {

class GltfLoader : public MeshLoader {

public:
  GltfLoader(PvBootstrap *bootstrap, std::shared_ptr<PvCommandPool> pool)
      : MeshLoader(bootstrap, pool) {}
  std::shared_ptr<PvMesh> loadModelFromFile(const char *file) override;

private:
  tinygltf::Model model;

  std::unique_ptr<PvMesh> loadModel(uint32_t index, bool storage_buffer);
};

} // namespace Pyra