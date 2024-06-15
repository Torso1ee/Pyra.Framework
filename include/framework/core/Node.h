#include <memory>
#include <vector>
namespace Pyra {
class Node {
protected:
  std::weak_ptr<Node> parent;
  std::vector<std::shared_ptr<Node>> childrens;

  virtual void constructMesh(){}
};
} // namespace Pyra