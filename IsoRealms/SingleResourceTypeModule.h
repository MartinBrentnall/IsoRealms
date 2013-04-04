#ifndef SINGLE_RESOURCE_TYPE_MODULE_H
#define SINGLE_RESOURCE_TYPE_MODULE_H

#include <string>
#include <vector>

#include "IPlugin.h"

template <class T> class SingleResourceTypeModule:public IPlugin {
  private:
  std::string cResourceTag;
  std::vector<T*> cResources;
    
  public:
  SingleResourceTypeModule(const std::string& resourceTag) {
    cResourceTag = resourceTag;
  }
  
  /**********************\
   * Implements IPlugin *
  \**********************/
  void createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
    for (int i = 0; i < node->getChildCount(); i++) {
      DOMNodeWrapper *mNode = node->getChild(i);
      std::string mValueAsString = mNode->getNodeName();
      if (mValueAsString == cResourceTag) {
        T* mResource = new T();
        cResources.push_back(mResource);
        std::string mResourceName = mNode->getAttribute("name");
        runtimeContext->add(mResource, mResourceName, mNode);
      }
    }
  }
  
  void save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
    for (unsigned int i = 0; i < cResources.size(); i++) {
      DOMNodeWriter* mResourceNode = node->addBranch(cResourceTag);
      std::string mResourceName = resourceLocator->getPath(cResources[i]);
      mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
      mResourceNode->addAttribute("name", mResourceName);
      cResources[i]->save(mResourceNode, resourceLocator);
    }
  }
};

#endif
