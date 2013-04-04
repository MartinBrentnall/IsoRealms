#include "ZoneCollectablesModule.h"

void ZoneCollectablesModule::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Collectables") {
      ZoneCollectables* mCollectables = new ZoneCollectables(this);
      cCollectables.push_back(mCollectables);
      std::string mName = mNode->getAttribute("name");
      IArgumentSource* mArgumentSource = new ArgumentSourceCustom<ZoneCollectables>(mCollectables);
      runtimeContext->add(mCollectables, mName, mNode);
      runtimeContext->registerArgument("ZoneCollectables", mName, mArgumentSource);
    }
  }
}

void ZoneCollectablesModule::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cCollectables.size(); i++) {
    DOMNodeWriter* mCollectablesNode = node->addBranch("Collectables");
    std::string mResourceName = resourceLocator->getPath(cCollectables[i]);
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mCollectablesNode->addAttribute("name", mResourceName);
    cCollectables[i]->save(mCollectablesNode, resourceLocator);
  }
}

void ZoneCollectablesModule::zoneContextChanged(IZone* zone) {
  cRuntimeZone = zone;
}

void ZoneCollectablesModule::zoneContextChanged(IMap* map, IZone* zone) {
  cEditingZone = zone;
}

void ZoneCollectablesModule::initPlugin(IZone* zone, unsigned int pass) {
  cEditingZone = zone;
}

IZone* ZoneCollectablesModule::getEditingZone() {
  return cEditingZone;
}

IZone* ZoneCollectablesModule::getRuntimeZone() {
  return cRuntimeZone;
}

extern "C" IPlugin* create() {
  return new ZoneCollectablesModule();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
