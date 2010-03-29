#ifndef DEFAULT_COLLIDABLE_SURFACE_REGISTRY_H
#define DEFAULT_COLLIDABLE_SURFACE_REGISTRY_H

#include <map>
#include <vector>

#include "../../../Global/IZone.h"

#include "../../ZoneContext/IZoneContext.h"
#include "../../ZoneContext/IZoneContextListener.h"

#include "../ICollidableSurfaceRegistry.h"

#include "SurfaceCache.h"

class DefaultCollidableSurfaceRegistry:public ICollidableSurfaceRegistry,
                                       public IZoneContextListener {
  private:
  std::map<IZone*, SurfaceCache*> cZoneSurfaceCaches;
  std::vector<PlugSocket*> cZoneContextSocket;
  IZoneContext* cZoneContext;
  IZone* cEditingZone;
  IZone* cRuntimeZone;

  public:
  DefaultCollidableSurfaceRegistry();

  /*****************************************\
   * Implements ICollidableSurfaceRegistry *
  \*****************************************/
  void registerRollableSurface(IRollableSurface*);
  void registerWallSurface(ICollidableWallSurface*);
  ICollisionData* getNextEvent(Vertex&, Vertex&);
  IRollableSurface* getSurfaceAt(Vertex&);

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
};

#endif
