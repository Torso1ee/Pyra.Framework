#pragma once
#include <memory>
#include "window/WindowBase.h"

namespace Pyra {

// base implementation for an application
class ApplicationBase {

public:
  /** @brief run app */
  void run();

  virtual void preRun(){}

  virtual void postRun(){}

  virtual void internalRun(){}

  virtual ~ApplicationBase() {}
};

}