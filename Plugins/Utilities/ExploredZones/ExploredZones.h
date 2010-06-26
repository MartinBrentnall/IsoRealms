#ifndef EXPLORED_ZONES_H
#define EXPLORED_ZONES_H

#include <set>
#include <vector>

#include "../IUtilities.h"

#include "../../CommandRegistry/ICommandRegistry.h"
#include "../../ZoneContext/IZoneContextListener.h"
#include "../../ZoneContext/IZoneContext.h"

class ExploredZones:public IUtilities,
                    public IZoneContextListener {
  private:
  IZoneContext* cZoneContext;
  ICommandRegistry* cCommandRegistry;
  std::set<IZone*> cExploredZones;
  unsigned int cZoneCount;
  std::vector<PlugSocket*> cSockets;
  std::vector<IUserCommand*> cAllZonesExploredCommands;
  std::vector<IUserCommand*> cZoneExploredCommands;
  
  public:
  ExploredZones();
  
  /**********************\
   * Implements IPlugin *
  \**********************/
  void initPlugin(IZone*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::string getName();
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*); 
  
  /***********************************\
   * Implements IZoneContextListener *
  \***********************************/
  void zoneContextChanged(IZone*);
};

#endif
