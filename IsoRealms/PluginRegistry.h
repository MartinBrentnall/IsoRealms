
/*
 * Copyright 2009 Martin Brentnall
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
#ifndef PLUGIN_REGISTRY_H
#define PLUGIN_REGISTRY_H

#include <dlfcn.h>
#include <map>
#include <string>

#include "Persistence/DOMNodeWriter.h"
#include "Hacks.h"
#include "IMap.h"
#include "InitException.h"
#include "IPlugin.h"
#include "IPluginRegistry.h"
#include "IZone.h"
#include "Resources/IResources.h"
#include "Resources/Registry.h"
#include "Resources/RuntimeContext.h"
#include "Resources/ZoneHandler/DefaultZoneHandler.h"
#include "System.h"

class Zone;

class PluginRegistry:public IPluginRegistry {
  private:
  // TODO: Change to const?
  static DefaultZoneHandler DEFAULT_ZONE_RENDERER;
    
  /**
   * Mapping of type to instance map; instance name to plug-in instances.
   */
  std::map<std::string, IPlugin*> cPluginInstances;
  std::map<IPlugin*, std::string> cImplementationNames;

  /**
   * Mapping of type, implementation to library handles.
   */
  std::map<std::string, std::map<std::string, void*> > cSOHandles;

  /**
   * Each plug-in maps to the corresponding destroy function found in it's
   * library.
   */
  std::map<IPlugin*, destroyPlugin*> cDestroyFunctions;

  public:

  /**
   * Get the type of the specified plugin instance.
   * 
   * @param IPlugin*  The plugin instance.
   * @returns  The type of the plugin.
   */
  std::string getPluginType(IPlugin*);  

  void registerPlugin(DOMNodeWrapper*, IResources*);

  /**
   * This allows all plugins to know that initialization is being performed for
   * the specified zone.  It may be called multiple times depending on how many
   * initialization passes are required for the zone.
   */
  void initPlugins(IZone*, unsigned int);

  /**
   * This function is called immediately prior to rendering a zone.  It allows
   * plugins to perform necessary set-up actions for rendering the specified
   * zone.
   * 
   * @param Zone*  The zone to be rendered.
   */
  void renderPreZone(IZone*);

  /**
   * This function is called when the zone editing context has changed.  It is
   * not used for the runtime.
   * 
   * @param Zone*  The new zone.
   */
  void zoneContextChanged(IMap*, IZone*);

  /**
   * Load a logic module.
   * 
   * @param string&  The logic type.
   */
  void loadPlugin(std::string&, IRuntimeContext*);

  void setEditingContext(IEditingContext*, IResourceManager*);
  
  /**
   * Remove the specified plugin instance.  All listeners will be notified of
   * the removal to give them an opportunity to relinquish use of the plugin
   * prior to removal.  The plugin should not be in use after the removal has
   * been completed.
   * 
   * @param IPlugin*  Instance to remove.
   */
  void removePlugin(IPlugin*);

  /**
   * Return instance names.
   * 
   * @returns  Instance names.
   */
  std::vector<std::string> getInstances();

  void save(DOMNodeWriter*, IResourceLocator*);

  void loadPluginData(DOMNodeWrapper*, IZone*);

  /**
   * All plugins should write any zone data.
   */
  void saveData(DOMNodeWriter*, IMap*, IZone*);
  
  /******************************\
   * Implements IPluginRegistry *
  \******************************/
  IPlugin* getPlugin(std::string&);
  std::string getInstanceName(IPlugin*);
  std::string getEntityPath(IPlugin*);

  ~PluginRegistry();
};

#endif
