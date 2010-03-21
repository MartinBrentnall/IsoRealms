#ifndef I_COLLECTABLES_ACCESSOR_H
#define I_COLLECTABLES_ACCESSOR_H

#include "../../Global/IElementSet.h"

#include "../../Plugins/Collectables/ICollectables.h"

class ICollectablesAccessor:public IElementSet {
  public:
  virtual ICollectables* getCollectables() = 0;
  virtual void jewelCollected() = 0;
  virtual void allJewelsCollected() = 0;
};

#endif
