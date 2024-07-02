#pragma once

namespace Pyra {

// base implementation for an application
class ApplicationBase {

public:
  /** @brief run app */
  void run();

  virtual void preRun() {}

  virtual void postRun() {}

  virtual void internalRun() {}
  
  virtual void registerEvent() {}

  virtual ~ApplicationBase() {}
};

} // namespace Pyra