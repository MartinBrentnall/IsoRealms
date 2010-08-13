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
  assignDummyPlugin(&cObjectives, "Objectives");
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  cZoneContextSocket.push_back(new PlugSocket("Objectives"));
  cZoneContextSocket.push_back(new PlugSocket("ZoneContext"));
  cCollectablesCount = 0;
  cCollectedCount = 0;
}

std::string ZoneCollectables::getName() {
  return "Zone Collectables";
}

std::vector<PlugSocket*> ZoneCollectables::getPlugSockets() {
  return cZoneContextSocket;
}

void ZoneCollectables::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "Objectives") {
    IObjectives* mPreviousObjectives = cObjectives;
    if (assignPlugin(plugin, &cObjectives, *socket)) {
      mPreviousObjectives->unregisterObjective(this);
      cObjectives->registerObjective(this);
    }
  } else if (socket->getType() == "ZoneContext") {
    IZoneContext* mPreviousZoneContext = cZoneContext;
    if (assignPlugin(plugin, &cZoneContext, *socket)) {
      mPreviousZoneContext->removeZoneContextListener(this);
      cZoneContext->addZoneContextListener(this);
      cEditingZone = cZoneContext->getZoneContext();
      cRuntimeZone = cEditingZone;
    }
  }
}

IPlugin* ZoneCollectables::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "ZoneContext") {return cZoneContext;}
  // TODO: Throw something
  return NULL;
}

void ZoneCollectables::zoneContextChanged(IZone* zone) {
  cRuntimeZone = zone;
}

void ZoneCollectables::notifyZoneAction(IZone* zone) {
  cEditingZone = zone;
}

void ZoneCollectables::initPlugin(IZone* zone, unsigned int pass) {
  cEditingZone = zone;
}

void ZoneCollectables::registerCollectable(ICollectable* collectable) {
  std::vector<ICollectable*>* mZoneCollectables = cCollectables[cEditingZone];
  if (mZoneCollectables == NULL) {
    mZoneCollectables = new std::vector<ICollectable*>();
    cCollectables[cEditingZone] = mZoneCollectables;
  }
  mZoneCollectables->push_back(collectable);
  cCollectablesCount++;
}

void ZoneCollectables::collect(ICollector* collector, Vertex& start, Vertex& end, IZone* zone) {
  std::map<IZone*, std::vector<ICollectable*>*>::iterator i = cCollectables.find(zone);
  if (i != cCollectables.end()) {
    for (unsigned int j = 0; j < i->second->size(); j++) {
      if ((*i->second)[j]->isCollected(start, end)) {
        collector->collected((*i->second)[j]);
        cCollectedCount++;
        cObjectives->check();
        // TODO: Remove collectable.
      }
    }
  }
}

void ZoneCollectables::collect(ICollector* collector, Vertex& start, Vertex& end) {
  if (cRuntimeZone != NULL) {
    collect(collector, start, end, cRuntimeZone);
  }
  std::vector<ZoneEvent*> mZoneEvents = cMap->getZoneEvents(start, end);
  for (unsigned int i = 0; i < mZoneEvents.size(); i++) {
    if (mZoneEvents[i]->getType() == ZoneEvent::ENTERED) {
      IZone* mEnteredZone = mZoneEvents[i]->getZone();
      collect(collector, start, end, mEnteredZone);
    }
  }
}

void ZoneCollectables::setRuntimeContext(IMap* map) {
  cMap = map;
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

bool ZoneCollectables::isMet() {
  return cCollectedCount == cCollectablesCount;
}

extern "C" IPlugin* create() {
  return new ZoneCollectables();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
