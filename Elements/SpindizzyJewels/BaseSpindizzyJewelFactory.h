#ifndef BASE_SPINDIZZY_JEWEL_FACTORY_H
#define BASE_SPINDIZZY_JEWEL_FACTORY_H

#include "../../Global/ElementFactory.h"

#include "ICollectablesAccessor.h"

class BaseSpindizzyJewelFactory:public ElementFactory<ICollectablesAccessor> {
  public:
  BaseSpindizzyJewelFactory(ICollectablesAccessor* elementSet) : ElementFactory<ICollectablesAccessor>(elementSet) {}
};

#endif
