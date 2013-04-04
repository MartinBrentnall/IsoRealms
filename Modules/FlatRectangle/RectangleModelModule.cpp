#include "RectangleModelModule.h"

void RectangleModelModule::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "ModelType") {
      FlatRectangleFactory* mResource = new FlatRectangleFactory();
      cResources.push_back(mResource);
      std::string mResourceName = mNode->getAttribute("name");
      runtimeContext->add(mResource, mResourceName, mNode);
      runtimeContext->addListener(mResource);
    }
  }
}
  
void RectangleModelModule::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cResources.size(); i++) {
    DOMNodeWriter* mResourceNode = node->addBranch("ModelType");
    std::string mResourceName = resourceLocator->getPath(cResources[i]);
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mResourceNode->addAttribute("name", mResourceName);
    cResources[i]->save(mResourceNode, resourceLocator);
  }
}

extern "C" IPlugin* create() {
  return new RectangleModelModule();
}

extern "C" void destroy(IPlugin* module) {
  delete module;
}
