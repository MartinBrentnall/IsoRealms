#ifndef EXPLORED_ZONES_H
#define EXPLORED_ZONES_H

#include <set>
#include <vector>

#include <IsoRealms/BlockArea.h>
#include <IsoRealms/IsoRealmsConstants.h>

#include "../IUtilities.h"

#include "../../CommandRegistry/ICommandRegistry.h"
#include "../../Objectives/IObjectives.h"
#include "../../ZoneContext/IZoneContextListener.h"
#include "../../ZoneContext/IZoneContext.h"

class ExploredZones:public IUtilities,
                    public IZoneContextListener,
                    public IObjective {
  private:
  IZoneContext* cZoneContext;
  ICommandRegistry* cCommandRegistry;
  IObjectives* cObjectives;
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
  void initPlugin(IZone*, unsigned int);
  IZoneRenderer* getZoneRenderer(const std::string&);
  void save(DOMNodeWriter*);
  void load(DOMNodeWrapper*);
  
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
  
  /*************************\
   * Implements IObjective *
  \*************************/
  bool isMet();
};

#endif
