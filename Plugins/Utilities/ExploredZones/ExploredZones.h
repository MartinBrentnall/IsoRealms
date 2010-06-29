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
  IZoneRenderer* cExploredZoneRenderer;
  IZoneRenderer* cMapOverviewRenderer;
  
  class ExploredZoneRenderer:public IZoneRenderer {
    private:
    ExploredZones* cParent;
    
    public:
    ExploredZoneRenderer(ExploredZones*);
    
    /****************************\
     * Implements IZoneRenderer *
    \****************************/
    void render(std::vector<IZone*>&);
  };
  
  class MapOverviewRenderer:public IZoneRenderer {
    private:
    ExploredZones* cParent;
    
    public:
    MapOverviewRenderer(ExploredZones*);
    
    /****************************\
     * Implements IZoneRenderer *
    \****************************/
    void render(std::vector<IZone*>&);
  };
  
  public:
  ExploredZones();
  
  /**********************\
   * Implements IPlugin *
  \**********************/
  void initPlugin(IZone*);
  IZoneRenderer* getZoneRenderer(const std::string&);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*); 
  
  /***********************************\
   * Implements IZoneContextListener *
  \***********************************/
  void zoneContextChanged(IZone*);
};

#endif
