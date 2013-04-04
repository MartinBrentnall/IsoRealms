#ifndef ELEMENT_MODEL_MODULE_H
#define ELEMENT_MODEL_MODULE_H

#include <IsoRealms/IPlugin.h>

#include "ElementModelType.h"

class ElementModelModule:public IPlugin {
  private:
  std::vector<ElementModelType*> cResources;
    
  public:
  void createResources(DOMNodeWrapper*, IRuntimeContext*);  
  void save(DOMNodeWriter*, IResourceLocator*);
};

#endif
