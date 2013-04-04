#ifndef I_ELEMENT_CONTAINER_H
#define I_ELEMENT_CONTAINER_H

#include "IElement.h"

class IElementContainer {
  public:
  virtual void addElement(IElement*) = 0;
  virtual void removeElement(IElement*) = 0;
};

#endif
