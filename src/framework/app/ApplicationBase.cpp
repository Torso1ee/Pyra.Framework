#include "app/ApplicationBase.h"

namespace Pyra {

void ApplicationBase::run() {
  preRun();
  internalRun();
  postRun();
}

} // namespace Pyra