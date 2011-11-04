
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

#include "Hacks.h"
#include "CommandDirectory.h"
#include "CommandRegistryProxy.h"
#include "DOMNodeWriter.h"
#include "DefaultZoneRenderer.h"
#include "IPlugin.h"
#include "IPluginRegistry.h"
#include "IPluginRegistryListener.h"
#include "IMap.h"
#include "InitException.h"
#include "IZone.h"
#include "RuntimeContext.h"
#include "System.h"
#include "ZoneRendererProxy.h"

class Zone;

class PluginRegistry:public IPluginRegistry {
  private:
  // TODO: Change to const?
  static DefaultZoneRenderer DEFAULT_ZONE_RENDERER;
    
  std::vector<IPluginRegistryListener*> cListeners;

  /**
   * Mapping of type to instance map; instance name to plug-in instances.
   */
  std::map<std::string, std::map<std::string, IPlugin*> > cPluginInstances;
  std::map<IPlugin*, std::string> cImplementationNames;

  /**
   * Mapping of type, implementation to library handles.
   */
  std::map<std::string, std::map<std::string, void*> > cSOHandles;

  /**
   * We keep these so we can save them and (TODO) destroy them.
   */
  std::vector<ZoneRendererProxy*> cZoneRendererProxies;
  
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

  void registerPlugin(DOMNodeWrapper*, CommandDirectory*, IMap*, bool);

  void connectPlugin(DOMNodeWrapper*);

  void loadConfiguration(DOMNodeWrapper*);

  void setPlugin(IPluginSupport*, DOMNodeWrapper*);

  /**
   * Called when a plug-in is removed.  The plug-in registry will check the
   * plug-ins to see which plug-ins may be using it, and will set reset any
   * such connections to NULL (typically the dummy plugin).
   * 
   * @param IPlugin*  The plugin instance to be removed.
   */
  void pluginRemoved(IPlugin*);

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
   * @param string&  The logic implementation.
   * @param string&  The name to assign to this instance.
   */
  void loadPlugin(std::string&, std::string&, std::string&);

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
   * Return instance names for the specified logic type.
   * 
   * @param string@  The logic type.
   * @returns  Instance names loaded for the logic type.
   */
  std::vector<std::string> getInstances(std::string&);

  /**
   * Add a listener to listen for logic instance changes.
   * 
   * @param IPluginRegistryListener*  The listener to add.
   */
  void addListener(IPluginRegistryListener*);

  /**
   * Remove a listener.
   * 
   * @param IPluginRegistryListener*  The listener to remove.
   */
  void removeListener(IPluginRegistryListener*);

  void save(DOMNodeWriter*);

  void loadPluginData(DOMNodeWrapper*, IZone*);

  /**
   * All plugins should write any zone data.
   */
  void saveData(DOMNodeWriter*, IMap*, IZone*);
  
  void saveZoneRenderers(DOMNodeWriter*);

  std::vector<IDynamicElement*> getPreLoopCommands();

  std::vector<IDynamicElement*> getPostLoopCommands();

  std::vector<IVisualElement*> getPreLoopRenderers();
  
  std::vector<IVisualElement*> getPostLoopRenderers();

  std::vector<IInteractiveElement*> getInteractiveElements();

  IZoneRenderer* getZoneRenderer(DOMNodeWrapper*, CommandDirectory*);
  
  /******************************\
   * Implements IPluginRegistry *
  \******************************/
  IPlugin* getPlugin(std::string&, std::string&);
  std::string getInstanceName(std::string, IPlugin*);
  std::string getEntityPath(IPlugin*);

  ~PluginRegistry();
};

#endif
