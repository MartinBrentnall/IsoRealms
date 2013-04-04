#ifndef I_CONTROLLER_H
#define I_CONTROLLER_H

#include <string>

class IController {
  public:
  virtual void startProject(const std::string&) = 0;
};

#endif
