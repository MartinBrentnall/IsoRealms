#ifndef I_SIZED_COMPONENT_H
#define I_SIZED_COMPONENT_H

#include "IComponentSizeCalculator.h"
#include "AbstractRectangularComponent.h"

class ISizedComponent:public AbstractRectangularComponent,
                      public IComponentSizeCalculator {
  public:
  virtual ~ISizedComponent() {}
};

#endif
