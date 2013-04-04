#ifndef I_COMPONENT_CUSTOM_TYPE_H
#define I_COMPONENT_CUSTOM_TYPE_H

#include "GUI/AbstractRectangularComponent.h"
#include "Persistence/DOMNodeWrapper.h"
#include "Resources/IResourceAccessor.h"

class IComponentCustomType {
  public:
  virtual AbstractRectangularComponent* createComponent(DOMNodeWrapper*, IResourceAccessor*) = 0;
};

#endif
