#include "pv/PvCommandPool.h"
#include <memory>
namespace Pyra {

class PvMesh;

class PvBootstrap;

class MeshLoader {

public:
  MeshLoader(PvBootstrap *bootstrap, std::shared_ptr<PvCommandPool> pool)
      : bootstrap(bootstrap), commandPool(pool) {}
  virtual std::shared_ptr<PvMesh> loadModelFromFile(const char *file) = 0;

protected:
  PvBootstrap *bootstrap;
  std::shared_ptr<PvCommandPool> commandPool;
};

} // namespace Pyra