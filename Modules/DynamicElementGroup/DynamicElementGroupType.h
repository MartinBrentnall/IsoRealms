#ifndef DYNAMIC_ELEMENT_GROUP_TYPE_H
#define DYNAMIC_ELEMENT_GROUP_TYPE_H

#include <map>

#include <IsoRealms/LuaSupport/ArgumentSourceCustom.h>
#include <IsoRealms/Resources/ElementType/IElementGroupType.h>
#include <IsoRealms/Resources/IRuntimeContext.h>
#include <IsoRealms/IPlugin.h>

#include "DynamicElementGroup.h"

class DynamicElementGroupType:public IElementGroupType,
                              public IPlugin {
  private:
  std::map<IElementContainer*, DynamicElementGroup*> cElementGroups;

  public:
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  void setActive(IZone*, bool);
  IElementGroup* getElementGroup(IElementContainer*);
};

#endif
