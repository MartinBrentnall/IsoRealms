#ifndef SPINDIZZY_CRAFT_PYRAMID_MODELS_H
#define SPINDIZZY_CRAFT_PYRAMID_MODELS_H

#include "SpindizzyCraftPyramidModelFactory.h"

class SpindizzyCraftPyramidModels:public IPlugin {
  private:
  std::vector<SpindizzyCraftPyramidModelFactory*> cResources;
    
  public:
  void createResources(DOMNodeWrapper*, IRuntimeContext*);  
  void save(DOMNodeWriter*, IResourceLocator*);
};

#endif
