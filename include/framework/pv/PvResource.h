#pragma once
#include "PvCommon.h"
#include "core/logging.h"
#include "core/trait.h"
#include <memory>
#include <tuple>
#include <unordered_map>

namespace Pyra {

class HandleManager {
public:
  ~HandleManager();
  static std::shared_ptr<HandleManager> get();
  static int count;

  void release();

private:
  static std::shared_ptr<HandleManager> handleManager;
};

class PvTable;

template <typename Handle, typename dctor> class PvResource {
public:
  static std::unordered_map<Handle, typename function_traits<dctor>::arg_types>
      handles;
  static dctor deconstuctor;
  Handle handle = nullptr;

  PvResource() { HandleManager::count += 1; }

  ~PvResource() {
    HandleManager::count -= 1;
    if (manageOp == MANUALLY_MANAGE)
      release(handle);
    if (HandleManager::count == 0)
      HandleManager::get()->release();
  }

  void manage(Handle hnd, typename function_traits<dctor>::arg_types funcParams,
              ManageOperation op = AUTO_MANAGE) {
    handle = hnd;
    if (op != DO_NOT_MANAGE) {
      handles[hnd] = funcParams;
    }
    manageOp = op;
  }

  static void release(Handle handle) {
    std::string address{printPointer(handle)};
    if (handle != nullptr) {
      INFO("manual destroy type[{}]:{}", typeid(Handle).name(), address);
      std::apply(deconstuctor, handles[handle]);
      handles.erase(handle);
    }
  }

  static void releaseAll() {
    if (handles.size() == 0)
      return;
    for (auto it = handles.begin(); it != handles.end();) {
      std::string address{printPointer(it->first)};
      INFO("auto destroy type[{}]:{}", typeid(Handle).name(), address);
      std::apply(deconstuctor, it->second);
      it = handles.erase(it);
    }
  }

protected:
  ManageOperation manageOp;
  PvTable *table;
};

template <typename Handle, typename dctor>
std::unordered_map<Handle, typename function_traits<dctor>::arg_types>
    PvResource<Handle, dctor>::handles;

template <typename Handle, typename dctor>
dctor PvResource<Handle, dctor>::deconstuctor = nullptr;

} // namespace Pyra