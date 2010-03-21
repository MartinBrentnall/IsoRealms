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
#include "IPluginSupport.h"

std::vector<PlugSocket*> IPluginSupport::cNoSockets;

IPlugin* IPluginSupport::getPlugin(PlugSocket* socket) {
  throwPluginSupportException(socket);
  return NULL;
}

IPlugin* IPluginSupport::getClientPlugin(PlugSocket* socket) {
  IPlugin* mPlugin = getPlugin(socket);
  return DummyPluginRegistry::isDummyPlugin(mPlugin) ? NULL : mPlugin;
}

std::vector<PlugSocket*> IPluginSupport::getPlugSockets() {
  return cNoSockets;
}

void IPluginSupport::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  throwPluginSupportException(socket);
}

void IPluginSupport::throwPluginSupportException(PlugSocket* socket) {
  throw new PluginSupportException("Plugin \"" + getName() + "\" does not support plugin of type \"" + socket->getType() + "\" with id \"" + socket->getID() + "\"");
}
