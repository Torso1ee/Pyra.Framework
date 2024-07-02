#include "pv/PvResource.h"
#include "pv/PvNode.h"

namespace Pyra {

std::shared_ptr<HandleManager> HandleManager::get() {
  if (handleManager == nullptr)
    handleManager = std::make_shared<HandleManager>();
  return handleManager;
}

std::shared_ptr<HandleManager> HandleManager::handleManager = nullptr;

void HandleManager::release() {
  for (auto p : nodeMap) {
    delete p.second;
  }
  nodeMap.clear();
}

void HandleManager::registerNode(PvNodeBase *node, std::vector<void *> deps) {
  nodeMap[node->handle] = node;
  for (auto d : deps) {
    if (d == nullptr)
      continue;
    if (nodeMap.contains(d)) {
      nodeMap[d]->childrens.push_back(node);
      node->parents.push_back(nodeMap[d]);
    }
  }
}

void HandleManager::release(void *handle) {
  auto node = nodeMap[handle];
  node->release();
}

HandleManager::~HandleManager() { release(); }

} // namespace Pyra