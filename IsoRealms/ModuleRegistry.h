
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
#ifndef MODULE_REGISTRY_H
#define MODULE_REGISTRY_H

#include <dlfcn.h>
#include <map>
#include <string>

#include "Configuration.h"
#include "Persistence/DOMNodeWriter.h"
#include "Hacks.h"
#include "InitException.h"
#include "IModule.h"
#include "IModuleRegistry.h"
#include "LuaSupport/ILuaModule.h"
#include "LuaSupport/ILuaSupport.h"
#include "Resources/IResources.h"
#include "Resources/Registry.h"
#include "Resources/RuntimeContext.h"
#include "System.h"

class ModuleRegistry:public IModuleRegistry {
  private:

  static ILuaSupport* cLuaSupport;
    
  /**
   * Mapping of type to instance map; instance name to plug-in instances.
   */
  std::map<std::string, IModule*> cModuleInstances;
  std::map<IModule*, std::string> cImplementationNames;

  /**
   * Mapping of type, implementation to library handles.
   */
  std::map<std::string, std::map<std::string, void*> > cSOHandles;

  /**
   * Each plug-in maps to the corresponding destroy function found in it's
   * library.
   */
  std::map<IModule*, destroyModule*> cDestroyFunctions;

  DOMNodeWrapper* getModuleOptions(const std::string&, DOMNodeWrapper*);
  
  public:

  /**
   * Get the type of the specified module instance.
   * 
   * @param IModule*  The module instance.
   * @returns  The type of the module.
   */
  std::string getModuleType(IModule*);  

  void registerModule(DOMNodeWrapper*, DOMNodeWrapper*, IResources*, IResourceTypeRegistry*, DOMNodeWrapper*);

  /**
   * Load a logic module.
   * 
   * @param string&  The logic type.
   */
  void loadModule(std::string&, IResourceRegistry*, IResourceTypeRegistry*);

  /**
   * Remove the specified module instance.  All listeners will be notified of
   * the removal to give them an opportunity to relinquish use of the module
   * prior to removal.  The module should not be in use after the removal has
   * been completed.
   * 
   * @param IModule*  Instance to remove.
   */
  void removeModule(IModule*);

  /**
   * Return instance names.
   * 
   * @returns  Instance names.
   */
  std::vector<std::string> getInstances();

  void save(DOMNodeWriter*, DOMNodeWriter*, IResourceLocator*);

  void projectInitialised();
  
  /******************************\
   * Implements IModuleRegistry *
  \******************************/
  IModule* getModule(std::string&);
  std::string getInstanceName(IModule*);
  std::string getEntityPath(IModule*);

  ~ModuleRegistry();
};

#endif
