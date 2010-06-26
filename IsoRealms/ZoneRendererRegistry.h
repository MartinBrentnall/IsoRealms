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
#ifndef ZONE_RENDERER_REGISTRY_H
#define ZONE_RENDERER_REGISTRY_H

#include <map>
#include <string>

#include "DefaultZoneRenderer.h"
#include "DOMNodeWrapper.h"
#include "IZoneRenderer.h"
#include "PluginRegistry.h"

class ZoneRendererRegistry {
  private:
  std::map<std::string, IZoneRenderer*> cZoneRenderers;
  
  std::map<std::string, std::string> cZoneRendererTypes;

  std::map<std::string, destroyZoneRenderer*> cDestroyFunctions;

  std::map<std::string, void*> cSOHandles;

  bool exists(const std::string&);
  
  public:
    
  /**
   * Create an zone renderer instance of the specified type and assign it the
   * specified name.  The instance name must be unique.
   * 
   * @param std::string&  Zone renderer implementation to create.
   * @param std::string&  Instance name to assign to the new zone renderer.
   * @return  The new element set instance.
   */
  IZoneRenderer* createInstance(const std::string&, const std::string&);

  IZoneRenderer* registerZoneRenderer(PluginRegistry&, DOMNodeWrapper*);
};

#endif
