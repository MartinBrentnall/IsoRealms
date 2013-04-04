#include "MappedZoneHandlers.h"

void MappedZoneHandlers::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Handler") {
      std::string mHandlerName = mNode->getAttribute("name");
      MappedZoneHandler* mHandler = new MappedZoneHandler();
      cZoneHandlers.push_back(mHandler);
      runtimeContext->add(mHandler, mHandlerName, mNode);
      IArgumentSource* mArgumentSource = new ArgumentSourceCustom<MappedZoneHandler>(mHandler);
      runtimeContext->registerArgument("Handler", mHandlerName, mArgumentSource);
    }
  }
}

void MappedZoneHandlers::initZone(IZone* zone) {
  for (unsigned int i = 0; i < cZoneHandlers.size(); i++) {
    cZoneHandlers[i]->setDefaultHandlers(zone);
  }
}

void MappedZoneHandlers::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cZoneHandlers.size(); i++) {
    DOMNodeWriter* mResourceNode = node->addBranch("Handler");
    std::string mResourceName = resourceLocator->getPath(cZoneHandlers[i]);
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mResourceNode->addAttribute("name", mResourceName);
    cZoneHandlers[i]->save(mResourceNode, resourceLocator);
  }
}

extern "C" IPlugin* create() {
  return new MappedZoneHandlers();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
