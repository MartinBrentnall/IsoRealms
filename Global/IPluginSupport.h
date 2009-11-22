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
#ifndef I_PLUGIN_SUPPORT_H
#define I_PLUGIN_SUPPORT_H

#include <string>
#include <vector>

#include "DOMNodeWrapper.h"
#include "PlugSocket.h"

class IPlugin;

class IPluginSupport {
  public:
  virtual ~IPluginSupport() {}

  /**
   * Return the name of the entity that requires the logic.
   * 
   * @returns  The name of the entity having these requirements.
   */
  virtual std::string getName() = 0;

  /**
   * Retrieve a list of the information about what plugins are supported by
   * entity.  Note that this list may change after a plugin has been set.
   * This may happen for example if enabling a plugin opens up possibilities
   * for other plugins to be supported.
   * 
   * @returns  The plug-ins supported.
   */
  virtual std::vector<PlugSocket*> getPlugSockets() = 0;

  /**
   * Set the implementation instance of a specified supported plugin.
   * 
   * @param PlugSocket*  The connection to set.
   * @param void*   Pointer to the implementing plugin.
   */
  virtual void setPlugin(PlugSocket*, IPlugin*) = 0;

  /**
   * Return the plug-in connected to the specified plug connection.
   * 
   * @param string  The requirement.
   * @returns       The name of the instance fulfilling the requirement.
   */
  virtual IPlugin* getPlugin(PlugSocket*) = 0;
};

typedef IPlugin* createPlugin(DOMNodeWrapper*);
typedef void destroyPlugin(IPlugin*);

#endif
