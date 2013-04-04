#ifndef I_COMPONENT_HOLDER_H
#define I_COMPONENT_HOLDER_H

#include <string>

#include "ISizedComponent.h"

class IComponentHolder {
  public:
  virtual void addComponent(const std::string&, ISizedComponent*) = 0;
  virtual void removeComponent(ISizedComponent*) = 0;
};

#endif
