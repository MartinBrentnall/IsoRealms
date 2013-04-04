#include "ScriptableVertices.h"

void ScriptableVertices::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Vertex") {
      ScriptableVertex* mVertex = new ScriptableVertex();
      cVertices.push_back(mVertex);
      std::string mName = mNode->getAttribute("name");
      IArgumentSource* mArgumentSource = new ArgumentSourceCustom<ScriptableVertex>(mVertex);
      runtimeContext->add(mVertex->getVertex(), mName);
      runtimeContext->registerArgument("Vertex", mName, mArgumentSource);
    }
  }
}

void ScriptableVertices::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cVertices.size(); i++) {
    DOMNodeWriter* mVertexNode = node->addBranch("Vertex");
    std::string mResourceName = resourceLocator->getPath(cVertices[i]->getVertex());
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mVertexNode->addAttribute("name", mResourceName);
  }
}

extern "C" IPlugin* create() {
  return new ScriptableVertices();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
