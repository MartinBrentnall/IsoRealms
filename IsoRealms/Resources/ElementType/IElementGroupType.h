#ifndef I_ELEMENT_GROUP_TYPE_H
#define I_ELEMENT_GROUP_TYPE_H

#include "IElementGroup.h"

class IElementGroupType {
  public:
  virtual IElementGroup* getElementGroup(IElementContainer*) = 0;
};

#endif
