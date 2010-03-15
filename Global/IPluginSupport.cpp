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
