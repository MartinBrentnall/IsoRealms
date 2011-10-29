/*
 * Copyright 2009,2010 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef EXPLORED_ZONES_H
#define EXPLORED_ZONES_H

#include <climits>
#include <set>
#include <vector>

#include <IsoRealms/BlockArea.h>
#include <IsoRealms/ICommandRegistry.h>
#include <IsoRealms/IPluginRegistry.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/IZoneRenderer.h>
#include <IsoRealms/Script.h>

#include "../IUtilities.h"

#include "../../3DModel/ISimpleModelFactory.h"
#include "../../FlaggedZones/IFlaggedZones.h"
#include "../../Objectives/IObjectives.h"
#include "../../StringProcessor/IStringProcessor.h"
#include "../../ZoneContext/IZoneContextListener.h"
#include "../../ZoneContext/IZoneContext.h"

class ExploredZones:public IUtilities,
                    public IZoneContextListener,
                    public IObjective {
  private:
  IZoneContext* cZoneContext;
  IZone* cZone;
  ICommandRegistry* cCommandRegistry;
  IObjectives* cObjectives;
  IStringProcessor* cToGoStringProcessor;
  std::string cToGoString;
  std::map<IZone*, ISimpleModel*> cExploredZones;
  unsigned int cZoneCount;
  std::vector<PlugSocket*> cSockets;
  Script* cAllZonesExploredScript;
  Script* cZoneExploredScript;
  IZoneRenderer* cExploredZoneRenderer;
  IZoneRenderer* cMapOverviewRenderer;
  ISimpleModelFactory* cFlagModel;
  IFlaggedZones* cFlaggedZones;
  
  class ExploredZoneRenderer:public IZoneRenderer {
    private:
    ExploredZones* cParent;
    
    public:
    ExploredZoneRenderer(ExploredZones*);
    
    /****************************\
     * Implements IZoneRenderer *
    \****************************/
    void render(std::vector<IZone*>&, IPluginRegistry&);
    void update(std::vector<IZone*>&, unsigned int);
    void updateRuntime(std::vector<IZone*>&, unsigned int);
  };
  
  class MapOverviewRenderer:public IZoneRenderer {
    private:
    ExploredZones* cParent;
    
    public:
    MapOverviewRenderer(ExploredZones*);
    
    /****************************\
     * Implements IZoneRenderer *
    \****************************/
    void render(std::vector<IZone*>&, IPluginRegistry&);
    void update(std::vector<IZone*>&, unsigned int);
    void updateRuntime(std::vector<IZone*>&, unsigned int);
  };
  
  void updateToGoString();
  
  public:
  ExploredZones();
  
  /**********************\
   * Implements IPlugin *
  \**********************/
  void initPlugin(IZone*, unsigned int);
  IZoneRenderer* getZoneRenderer(const std::string&);
  void save(DOMNodeWriter*);
  void load(DOMNodeWrapper*);
  void setRuntimeContext(IRuntimeContext*);
  
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
