#ifndef SPINDIZZY_JEWEL_MODEL_MODULE_H
#define SPINDIZZY_JEWEL_MODEL_MODULE_H

#include <IsoRealms/IPlugin.h>

#include "SpindizzyJewelModelFactory.h"

class SpindizzyJewelModelModule:public IPlugin {
  private:
  std::vector<SpindizzyJewelModelFactory*> cResources;
    
  public:
  void createResources(DOMNodeWrapper*, IRuntimeContext*);  
  void save(DOMNodeWriter*, IResourceLocator*);
};

#endif
