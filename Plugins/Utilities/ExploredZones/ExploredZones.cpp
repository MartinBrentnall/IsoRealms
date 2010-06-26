#include "ExploredZones.h"

ExploredZones::ExploredZones() {
  assignDummyPlugin(&cCommandRegistry, "CommandRegistry");
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  cSockets.push_back(new PlugSocket("CommandRegistry"));
  cSockets.push_back(new PlugSocket("ZoneContext"));
  cZoneCount = 0;
}

void ExploredZones::initPlugin(IZone* zone) {
  cZoneCount++;
}

void ExploredZones::zoneContextChanged(IZone* zone) {
  if (cExploredZones.find(zone) == cExploredZones.end()) {
    cExploredZones.insert(zone);
    for (unsigned int i = 0; i < cZoneExploredCommands.size(); i++) {
      cZoneExploredCommands[i]->execute();
    }
    if (cExploredZones.size() == cZoneCount) {
      for (unsigned int i = 0; i < cAllZonesExploredCommands.size(); i++) {
        cAllZonesExploredCommands[i]->execute();
      }
    }
  }
}

std::string ExploredZones::getName() {
  return "Explored Zones";
}

std::vector<PlugSocket*> ExploredZones::getPlugSockets() {
  return cSockets;
}

void ExploredZones::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "CommandRegistry") {
    assignPlugin(plugin, &cCommandRegistry, *socket);
  } else if (socket->getType() == "ZoneContext") {
    assignPlugin(plugin, &cZoneContext, *socket);
  } else {
    // TODO: Throw
  }
}

IPlugin* ExploredZones::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "CommandRegistry") {return cCommandRegistry;}
  if (socket->getType() == "ZoneContext")     {return cZoneContext;}
  return NULL;
}

extern "C" IPlugin* create() {
  return new ExploredZones();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
