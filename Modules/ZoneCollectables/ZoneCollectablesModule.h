#ifndef SPINDIZZY_CAMERAS_H
#define SPINDIZZY_CAMERAS_H

#include <IsoRealms/LuaSupport/ArgumentSourceCustom.h>
#include <IsoRealms/IPlugin.h>
#include <IsoRealms/IZoneContextListener.h>

#include "IZoneCollectables.h"
#include "ZoneCollectables.h"

class ZoneCollectablesModule:public IPlugin,
                             public IZoneCollectables,
                             public IZoneContextListener {
  private:
  std::vector<ZoneCollectables*> cCollectables;
  IZone* cEditingZone;
  IZone* cRuntimeZone;
  
  public:
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  void save(DOMNodeWriter*, IResourceLocator*);
  
  IZone* getEditingZone();
  IZone* getRuntimeZone();
  
  void zoneContextChanged(IZone*);
  void zoneContextChanged(IMap*, IZone*);
  void initPlugin(IZone*, unsigned int);
};

#endif
