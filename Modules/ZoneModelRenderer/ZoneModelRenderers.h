#ifndef ZONE_MODEL_RENDERERS_H
#define ZONE_MODEL_RENDERERS_H

#include <string>

#include <IsoRealms/IPlugin.h>

#include "ZoneModelRenderer.h"

class ZoneModelRenderers:public IPlugin {
  private:
  std::vector<ZoneModelRenderer*> cRenderers;
  
  public:
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  void save(DOMNodeWriter*, IResourceLocator*);
};

#endif
