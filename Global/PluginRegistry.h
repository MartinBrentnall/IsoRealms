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
#include "DOMNodeWriter.h"
#include "IPlugin.h"
#include "IPluginRegistryListener.h"
#include "InitException.h"
#include "System.h"

class Zone;

class PluginRegistry {
  private:
  static std::map<std::string, IPlugin*> cDummyPlugins;  

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
   * Each plug-in maps to the corresponding destroy function found in it's
   * library.
   */
  std::map<IPlugin*, destroyPlugin*> cDestroyFunctions;

  public:

  void registerPlugin(DOMNodeWrapper*);

  void setPlugin(IPlugin*, DOMNodeWrapper*);

  /**
   * This allows all plugins to know that an editor action is being performed
   * on the specified zone.  It will be called immediately prior to the editing
   * action being carried out.
   */
  void notifyZoneAction(Zone*);

  /**
   * This allows all plugins to know that initialization is being performed for
   * the specified zone.  It may be called multiple times depending on how many
   * initialization passes are required for the zone.
   */
  void initPlugins(Zone*);

  /**
   * Load a logic module.
   * 
   * @param string&  The logic type.
   * @param string&  The logic implementation.
   * @param string&  The name to assign to this instance.
   */
  void loadPlugin(std::string&, std::string&, std::string&);

  /**
   * Return instance names for the specified logic type.
   * 
   * @param string@  The logic type.
   * @returns  Instance names loaded for the logic type.
   */
  std::vector<std::string> getInstances(std::string&);

  /**
   * Retrieve a logic factory instance of the specified type and name.
   * 
   * @param string&  The logic type.
   * @param string&  The logic instance name.
   * @returns  The instance of the logic.
   */
  IPlugin* getPlugin(std::string&, std::string&);

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

  static IPlugin* getDummyPlugin(std::string&);

  static bool isDummyPlugin(IPlugin*);

  /**
   * Called when a plug-in is removed.  The plug-in registry will check the
   * plug-ins to see which plug-ins may be using it, and will set reset any
   * such connections to NULL (typically the dummy plugin).
   * 
   * @param IPlugin*  The plugin instance to be removed.
   */
  void pluginRemoved(IPlugin*);

  /**
   * Retrieve the plug-in instance name
   */
  std::string getInstanceName(std::string, IPlugin*);

  void save(DOMNodeWriter*);

  ~PluginRegistry();
};

#endif
