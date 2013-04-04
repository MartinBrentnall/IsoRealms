#include "DynamicElementGroupType.h"

void DynamicElementGroupType::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  IArgumentSource* mArgumentSource = new ArgumentSourceCustom<DynamicElementGroupType>(this);
  runtimeContext->registerArgument("Type", "Zones", mArgumentSource);
  runtimeContext->add(this, "Type");
}

void DynamicElementGroupType::setActive(IZone* zone, bool active) {
  std::cout << "Zone " << zone << " is active?  " << active << std::endl;
  DynamicElementGroup* mGroup = cElementGroups[zone];
  if (mGroup != NULL) {
    mGroup->setActive(active);
  }
}

IElementGroup* DynamicElementGroupType::getElementGroup(IElementContainer* container) {
  DynamicElementGroup* mGroup = cElementGroups[container];
  if (mGroup == NULL) {
    mGroup = new DynamicElementGroup();
    cElementGroups[container] = mGroup;
  }
  return mGroup;
}

extern "C" IPlugin* create() {
  return new DynamicElementGroupType();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
