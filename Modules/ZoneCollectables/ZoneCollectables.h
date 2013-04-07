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
#include <sstream>
#include <vector>

#include <IsoRealms/IMap.h>
#include <IsoRealms/IZone.h>
#include <IsoRealms/LuaSupport/ArgumentSourceLocal.h>
#include <IsoRealms/LuaSupport/ArgumentValue.h>
#include <IsoRealms/LuaSupport/IArgumentGenerator.h>
#include <IsoRealms/Resources/Collectables/ICollectables.h>
#include <IsoRealms/Resources/Integer/Integer.h>
#include <IsoRealms/SingleResourceTypeModule.h>

#include "IZoneCollectables.h"

class ZoneCollectables:public ICollectables,
                       public IArgumentGenerator,
		       public IArgumentLocator {
  private:
  IMap* cMap;
  IZoneCollectables* cModule;
  IScript* cCollectedScript;
  ArgumentValue<IInteger> cArgumentZoneRemaining;
  ArgumentValue<IZone> cArgumentZone;
  std::map<IZone*, std::vector<ICollectable*>*> cCollectables;
  unsigned int cCollectablesCount;
  unsigned int cCollectedCount;
  std::string cCollectedCountString;

  void collect(ICollector*, Vertex&, Vertex&, IZone*);
  
  public:
  ZoneCollectables(IZoneCollectables*);
  
  int getRemaining(IZone*);

  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  void save(DOMNodeWriter*, IResourceLocator*);
  
  /*************************\
   * Implements IObjective *
  \*************************/
  bool isMet();
  
  /**********************************\
   * Implements IFlaggedZonesSource *
  \**********************************/
  bool isZoneFlagged(IZone*);

  /****************************\
   * Implements ICollectables *
  \****************************/
  void registerCollectable(ICollectable*);
  void collect(ICollector*, Vertex&, Vertex&);
  void reinitialise();
  
  IArgumentSource* getArgument(DOMNodeWrapper*);
  std::string getPath(IArgumentValue*);
};

#endif
