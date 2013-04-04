#include "ZoneModelRenderers.h"

void ZoneModelRenderers::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Renderer") {
      ZoneModelRenderer* mRenderer = new ZoneModelRenderer();
      cRenderers.push_back(mRenderer);
      std::string mName = mNode->getAttribute("name");
      runtimeContext->add(mRenderer, mName, mNode);
    }
  }
}

void ZoneModelRenderers::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cRenderers.size(); i++) {
    DOMNodeWriter* mRendererNode = node->addBranch("Renderer");
    std::string mResourceName = resourceLocator->getPath(cRenderers[i]);
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mRendererNode->addAttribute("name", mResourceName);
    cRenderers[i]->save(mRendererNode, resourceLocator);
  }
}

extern "C" IPlugin* create() {
  return new ZoneModelRenderers();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
