#ifndef MAPPED_ZONE_RENDERERS_H
#define MAPPED_ZONE_RENDERERS_H

#include <map>
#include <vector>

#include <IsoRealms/LuaSupport/ArgumentSourceCustom.h>
#include <IsoRealms/Resources/ZoneHandler/IZoneHandler.h>

#include "MappedZoneHandler.h"

class MappedZoneHandlers:public IPlugin {
  private:
  std::vector<MappedZoneHandler*> cZoneHandlers;
  
  public:
  void createResources(DOMNodeWrapper*, IRuntimeContext*);

  void save(DOMNodeWriter*, IResourceLocator*);
  
  void initZone(IZone*);
  
};

#endif
