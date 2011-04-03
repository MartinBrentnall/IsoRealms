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

#include <iostream>
#include <string>
#include <vector>

#include "DOMNodeWrapper.h"
#include "DummyPluginRegistry.h"
#include "PluginSupportException.h"
#include "PlugSocket.h"

class IPlugin;

class IPluginSupport {
  private:
  static std::vector<PlugSocket*> cNoSockets;

  virtual IPlugin* getPlugin(PlugSocket*);

  void throwPluginSupportException(PlugSocket*);

  protected:

  /**
   * Utility method for assigning plugin implementation to a destination
   * pointer.  If the assignment fails, the destination pointer is unchanged.
   * 
   * @param source  The plugin to assign from.
   * @param dest  Pointer to the plugin type to assign to.
   * @param PlugSocket&  The socket representing the destination.
   * @param bool  true to use dummy implementation (NULL object) when the
   *              source is NULL, otherwise the dest is set to NULL.
   * @returns  true if assignment was successful, otherwise false to indicate
   *           that the assignment failed (i.e. both source and dest were the
   *           same, or the source couldn't be cast to the destination type).
   */
  template<class T> bool assignPlugin(IPlugin* source, T** dest, PlugSocket& socket, bool useDummy = true) {
    if (source == *dest || (source == NULL && DummyPluginRegistry::isDummyPlugin(*dest))) {
      std::cout << "WARNING: Source and destination plugins are identical!" << std::endl;
      return false;
    }

    if (source == NULL) {
      if (!useDummy) {
        *dest = NULL;
        return true;
      }
      source = DummyPluginRegistry::getDummyPlugin(socket.getType());
    }

    T* mTypedSource = dynamic_cast<T*>(source);
    if (mTypedSource == NULL) {
      // TODO: Throw exception
      std::cout << "WARNING: Set plugin failed for plugin type \"" + socket.getType() + "\"" << std::endl;
      return false;
    }
    *dest = mTypedSource;
    return true;
  }

  template<class T> void assignDummyPlugin(T** dest, const std::string& type) {
    *dest = dynamic_cast<T*>(DummyPluginRegistry::getDummyPlugin(type));
    if (*dest == NULL) {
      // TODO: Throw wobbly
      std::cout << "WARNING: Setting dummy plugin failed for type \"" << type << "\"!" << std::endl;
    }
  }

  public:

  /**
   * Retrieve a list of the information about what plugins are supported by
   * entity.  Note that this list may change after a plugin has been set.
   * This may happen for example if enabling a plugin opens up possibilities
   * for other plugins to be supported.
   * 
   * @returns  The plug-ins supported.
   */
  virtual std::vector<PlugSocket*> getPlugSockets();

  /**
   * Set the implementation instance of a specified supported plugin.
   * 
   * @param PlugSocket*  The connection to set.
   * @param void*   Pointer to the implementing plugin.
   */
  virtual void setPlugin(PlugSocket*, IPlugin*);

  /**
   * Return the plug-in connected to the specified plug connection.
   * 
   * @param string  The requirement.
   * @returns       The name of the instance fulfilling the requirement.
   */
  IPlugin* getClientPlugin(PlugSocket*);

  virtual ~IPluginSupport() {}
};

#endif
