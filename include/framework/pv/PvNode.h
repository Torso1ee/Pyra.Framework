#pragma once
#include "core/trait.h"
#include <algorithm>
#include <memory.h>
#include <string>
#include <vector>


namespace Pyra {

class PvNodeBase {
public:
  PvNodeBase(void *h) : handle(h) {}

  std::vector<PvNodeBase *> childrens;
  std::vector<PvNodeBase *> parents;
  void *handle;
  bool requestRelease = false;
  bool released = false;

  void remove(void *handle) {
    auto it = std::find_if(childrens.cbegin(), childrens.cend(),
                           [&handle](auto pr) { return pr->handle == handle; });
    if (it != childrens.cend()) {
      childrens.erase(it);
    }
  }

  virtual void release() {}
};

template <typename Handle, typename dctor> class PvNode : public PvNodeBase {

public:
  typename function_traits<dctor>::arg_types args;

  static dctor deconstructor;

  PvNode(void *h, function_traits<dctor>::arg_types arg)
      : args(arg), PvNodeBase(h) {}

  void release() override {
    std::string address{printPointer(handle)};
    INFO("requestRelease type[{}]:{}", typeid(Handle).name(), address);
    requestRelease = true;
    if (childrens.empty()) {
      std::apply(deconstructor, args);
      released = true;
      INFO("destroy type[{}]:{}", typeid(Handle).name(), address);
      for (auto p : parents) {
        auto it = std::find_if(
            p->childrens.cbegin(), p->childrens.cend(),
            [this](auto pr) { return pr->handle == this->handle; });
        if (it != p->childrens.cend()) {
          p->childrens.erase(it);
        }
        if (p->requestRelease)
          p->release();
      }
    }
  }
};

template <typename Handle, typename dctor>
dctor PvNode<Handle, dctor>::deconstructor = nullptr;
} // namespace Pyra.