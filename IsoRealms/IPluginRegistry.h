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
#ifndef I_PLUGIN_REGISTRY_H
#define I_PLUGIN_REGISTRY_H

#include <string>

class IPlugin;
class IZone;

class IPluginRegistry {
  public:

  /**
   * Retrieve a logic factory instance of the specified type and name.
   * 
   * @param string&  The logic type.
   * @param string&  The logic instance name.
   * @returns  The instance of the logic.
   */
  virtual IPlugin* getPlugin(std::string&, std::string&) = 0;

  /**
   * Retrieve the plug-in instance name TODO
   */
  virtual std::string getInstanceName(std::string, IPlugin*) = 0;
  
  /**
   * Get the path of the specified plugin TODO
   */
  virtual std::string getEntityPath(IPlugin*) = 0;
  
  // TODO: Remove this
  virtual void renderPreZone(IZone*) = 0;
};

#endif
