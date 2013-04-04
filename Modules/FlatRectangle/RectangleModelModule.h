#ifndef RECTANGLE_MODEL_MODULE_H
#define RECTANGLE_MODEL_MODULE_H

#include <IsoRealms/IPlugin.h>
#include <IsoRealms/Resources/IRuntimeContext.h>

#include "FlatRectangleFactory.h"

class RectangleModelModule:public IPlugin {
  private:
  std::vector<FlatRectangleFactory*> cResources;
  
  public:
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  void save(DOMNodeWriter*, IResourceLocator*);
};

#endif
