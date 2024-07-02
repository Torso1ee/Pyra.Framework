#pragma once
#include "PvCommon.h"
#include "core/trait.h"
#include "pv/PvNode.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace Pyra {

class HandleManager {
public:
  ~HandleManager();
  static std::shared_ptr<HandleManager> get();
  static int count;

  std::unordered_map<void *, PvNodeBase *> nodeMap;

  void release();

  void registerNode(PvNodeBase *node, std::vector<void *> deps = {});

  void release(void *);

private:
  static std::shared_ptr<HandleManager> handleManager;
};

class PvTable;

template <typename Handle, typename dctor> class PvResource {
public:
  Handle handle = nullptr;

  ~PvResource() {
    if (manageOp != DO_NOT_MANAGE)
      HandleManager::get()->release(handle);
  }

  void manage(Handle hnd, typename function_traits<dctor>::arg_types funcParams,
              ManageOperation op = AUTO_MANAGE, std::vector<void *> deps = {}) {
    handle = hnd;
    if (op != DO_NOT_MANAGE) {
      auto node = new PvNode<Handle, dctor>(handle, funcParams);
      HandleManager::get()->registerNode(node, deps);
    }
    manageOp = op;
  }

protected:
  ManageOperation manageOp;
  PvTable *table;
  static inline bool setDctor = false;

  virtual void archiveData(){}

  static void setDeconstructor(dctor dt) {
    if(dt != nullptr){
    PvNode<Handle, dctor>::deconstructor = dt;
    setDctor = true;
    }
  }
};

} // namespace Pyra