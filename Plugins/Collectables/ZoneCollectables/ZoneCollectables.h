#ifndef ZONE_COLLECTABLES_H
#define ZONE_COLLECTABLES_H

#include <map>
#include <vector>

#include "../../../Global/IZone.h"

#include "../../ZoneContext/IZoneContext.h"

#include "../ICollectables.h"

class ZoneCollectables:public ICollectables,
                       public IZoneContextListener {
  private:
  IZoneContext* cZoneContext;
  IZone* cCurrentZone;
  std::vector<PlugSocket*> cZoneContextSocket;
  std::map<IZone*, std::vector<ICollectable*>*> cCollectables;

  public:
  ZoneCollectables();

  /***********************************\
   * Implements IZoneContextListener *
  \***********************************/
  void zoneContextChanged(IZone*);

  /**********************\
   * Implements IPlugin *
  \**********************/
  void notifyZoneAction(IZone*);
  void initPlugin(IZone*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::string getName();
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /****************************\
   * Implements ICollectables *
  \****************************/
  void registerCollectable(ICollectable*);
  void collect(ICollector*, Vertex&, Vertex&);
  void reinitialise();
};

#endif
