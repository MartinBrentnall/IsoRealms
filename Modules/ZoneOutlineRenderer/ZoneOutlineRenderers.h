#ifndef ZONE_OUTLINE_RENDERERS_H
#define ZONE_OUTLINE_RENDERERS_H

#include <IsoRealms/LuaSupport/ArgumentSource.h>
#include <IsoRealms/IPlugin.h>

#include "ZoneOutlineRenderer.h"

class ZoneOutlineRenderers:public IPlugin {
  private:
  std::vector<ZoneOutlineRenderer*> cRenderers;
  
  public:
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  void save(DOMNodeWriter*, IResourceLocator*);
};

#endif
