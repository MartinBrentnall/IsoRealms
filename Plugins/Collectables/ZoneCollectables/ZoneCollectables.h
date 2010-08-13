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
#ifndef ZONE_COLLECTABLES_H
#define ZONE_COLLECTABLES_H

#include <map>
#include <vector>

#include <IsoRealms/IMap.h>
#include <IsoRealms/IZone.h>

#include "../../Objectives/IObjectives.h"
#include "../../ZoneContext/IZoneContext.h"

#include "../ICollectables.h"

class ZoneCollectables:public ICollectables,
                       public IObjective,
                       public IZoneContextListener {
  private:
  IZoneContext* cZoneContext;
  IObjectives* cObjectives;
  IZone* cEditingZone;
  IZone* cRuntimeZone;
  IMap* cMap;
  std::vector<PlugSocket*> cZoneContextSocket;
  std::map<IZone*, std::vector<ICollectable*>*> cCollectables;
  unsigned int cCollectablesCount;
  unsigned int cCollectedCount;

  void collect(ICollector*, Vertex&, Vertex&, IZone*);
  
  public:
  ZoneCollectables();

  /***********************************\
   * Implements IZoneContextListener *
  \***********************************/
  void zoneContextChanged(IZone*);
  
  /*************************\
   * Implements IObjective *
  \*************************/
  bool isMet();

  /**********************\
   * Implements IPlugin *
  \**********************/
  void notifyZoneAction(IZone*);
  void initPlugin(IZone*, unsigned int);
  void setRuntimeContext(IMap*);

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
