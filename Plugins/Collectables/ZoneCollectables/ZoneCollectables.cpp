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
#include "ZoneCollectables.h"

ZoneCollectables::ZoneCollectables() {
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  cZoneContextSocket.push_back(new PlugSocket("ZoneContext"));
}

std::string ZoneCollectables::getName() {
  return "Zone Collectables";
}

std::vector<PlugSocket*> ZoneCollectables::getPlugSockets() {
  return cZoneContextSocket;
}

void ZoneCollectables::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "ZoneContext") {
    IZoneContext* mPreviousZoneContext = cZoneContext;
    if (assignPlugin(plugin, &cZoneContext, *socket)) {
      mPreviousZoneContext->removeZoneContextListener(this);
      cZoneContext->addZoneContextListener(this);
      cCurrentZone = cZoneContext->getZoneContext();
    }
  }
}

IPlugin* ZoneCollectables::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "ZoneContext") {return cZoneContext;}
  // TODO: Throw something
  return NULL;
}

void ZoneCollectables::zoneContextChanged(IZone* zone) {
  cCurrentZone = zone;
}

void ZoneCollectables::notifyZoneAction(IZone* zone) {
  cCurrentZone = zone;
}

void ZoneCollectables::initPlugin(IZone* zone) {
  cCurrentZone = zone;
}

void ZoneCollectables::registerCollectable(ICollectable* collectable) {
  std::vector<ICollectable*>* mZoneCollectables = cCollectables[cCurrentZone];
  if (mZoneCollectables == NULL) {
    mZoneCollectables = new std::vector<ICollectable*>();
    cCollectables[cCurrentZone] = mZoneCollectables;
  }
  mZoneCollectables->push_back(collectable);  
}

void ZoneCollectables::collect(ICollector* collector, Vertex& start, Vertex& end) {
  if (cCurrentZone != NULL) {
    std::map<IZone*, std::vector<ICollectable*>*>::iterator i = cCollectables.find(cCurrentZone);
      if (i != cCollectables.end()) {
      for (unsigned int j = 0; j < i->second->size(); j++) {
        if ((*i->second)[j]->isCollected(start, end)) {
          collector->collected((*i->second)[j]);
          // TODO: Remove collectable?
        }
      }
    }
  }
}

void ZoneCollectables::reinitialise() {
  for (std::map<IZone*, std::vector<ICollectable*>*>::iterator i = cCollectables.begin(); i != cCollectables.end(); ++i) {
    for (unsigned int j = 0; j < i->second->size(); j++) {
      (*i->second)[j]->setDirty();
    }
    delete i->second;
  }
  cCollectables.clear();
}

extern "C" IPlugin* create() {
  return new ZoneCollectables();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
