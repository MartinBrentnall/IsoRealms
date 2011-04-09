/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#ifndef ZONE_CONTEXT_RENDERER_H
#define ZONE_CONTEXT_RENDERER_H

#include <IsoRealms/IPluginRegistry.h>
#include <IsoRealms/IZoneRenderer.h>

#include "../../ZoneContext/IZoneContext.h"
#include "../../ZoneContext/IZoneContextListener.h"

#include "../IUtilities.h"

class ZoneContextRenderer:public IUtilities,
                          public IZoneContextListener,
                          public IZoneRenderer {
  private:
  std::vector<PlugSocket*> cSockets;
  IZoneContext* cZoneContext;
  IZone* cZone;  
  
  public:
  ZoneContextRenderer();
  
  /**********************\
   * Implements IPlugin *
  \**********************/
  IZoneRenderer* getZoneRenderer(const std::string&);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*); 
  
  /****************************\
   * Implements IZoneRenderer *
  \****************************/
  void render(std::vector<IZone*>&, IPluginRegistry&);
  void update(std::vector<IZone*>&, unsigned int);
  void updateRuntime(std::vector<IZone*>&, unsigned int);

  /***********************************\
   * Implements IZoneContextListener *
  \***********************************/
  void zoneContextChanged(IZone*);
};

#endif
