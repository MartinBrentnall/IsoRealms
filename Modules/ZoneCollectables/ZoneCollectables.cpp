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

ZoneCollectables::ZoneCollectables(IZoneCollectables* module) {
  cModule = module;
  cCollectablesCount = 0;
  cCollectedCount = 0;
  cCollectedCountString = "0";
  cCollectedScript = NULL;
}

void ZoneCollectables::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cMap = resources->getMap(node->getAttribute("map"));
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "CollectedScript") {
      cCollectedScript = resources->getLuaScript(mNode, this);
    } else {
      // TODO: Throw
    }
  }
}

void ZoneCollectables::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  DOMNodeWriter* mCollectedScriptNode = node->addBranch("CollectedScript");
  cCollectedScript->save(mCollectedScriptNode, resourceLocator);
}

int ZoneCollectables::getRemaining(IZone* zone) {
  std::map<IZone*, std::vector<ICollectable*>*>::iterator i = cCollectables.find(zone);
  if (i != cCollectables.end()) {
    return i->second->size();
  }
  return 0;
}

void ZoneCollectables::registerCollectable(ICollectable* collectable) {
  IZone* mZone = cModule->getEditingZone();
  std::cout << "Registering collectable to zone " << mZone << std::endl;
  std::vector<ICollectable*>* mZoneCollectables = cCollectables[mZone];
  if (mZoneCollectables == NULL) {
    mZoneCollectables = new std::vector<ICollectable*>();
    cCollectables[mZone] = mZoneCollectables;
  }
  mZoneCollectables->push_back(collectable);
  cCollectablesCount++;
}

void ZoneCollectables::collect(ICollector* collector, Vertex& start, Vertex& end, IZone* zone) {
  std::map<IZone*, std::vector<ICollectable*>*>::iterator i = cCollectables.find(zone);
  if (i != cCollectables.end()) {
    for (int j = i->second->size() - 1; j >= 0; j--) {
      if ((*i->second)[j]->isCollected(start, end)) {
        collector->collected((*i->second)[j]);
        std::stringstream mStringStream;
        mStringStream << ++cCollectedCount;
        cCollectedCountString = mStringStream.str();
//        cObjectives->check();
        i->second->erase(i->second->begin() + j);
        Integer mZoneRemaining(i->second->size());
        cArgumentZoneRemaining.setValue(&mZoneRemaining);
        cArgumentZone.setValue(zone);
        cCollectedScript->execute();
      }
    }
  }
}

void ZoneCollectables::collect(ICollector* collector, Vertex& start, Vertex& end) {
  IZone* mZone = cMap->getZone(start);
  if (mZone != NULL) {
    collect(collector, start, end, mZone);
  }
  std::vector<ZoneEvent*> mZoneEvents = cMap->getZoneEvents(start, end);
  for (unsigned int i = 0; i < mZoneEvents.size(); i++) {
    if (mZoneEvents[i]->getType() == ZoneEvent::ENTERED) {
      IZone* mEnteredZone = mZoneEvents[i]->getZone();
      collect(collector, start, end, mEnteredZone);
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

bool ZoneCollectables::isMet() {
  return cCollectedCount == cCollectablesCount;
}

std::string ZoneCollectables::getPath(IArgumentValue* value) {
  return value == &cArgumentZoneRemaining ? "zoneRemaining"
       : value == &cArgumentZone          ? "zone"
       :                                    "";
}

IArgumentSource* ZoneCollectables::getArgument(DOMNodeWrapper* node) {
  std::string mType = node->getAttribute("type");
  std::string mInstance = node->getAttribute("value").substr(1);
  std::string mName = node->getAttribute("name");
  if (mType == "Integer") {
    if (mInstance == "zoneRemaining") {
      return new ArgumentSourceLocal<IInteger>(&cArgumentZoneRemaining, this);
    }
  } else if (mType == "Zone") {
    if (mInstance == "zone") {
      return new ArgumentSourceLocal<IZone>(&cArgumentZone, this);
    }
  }
  return NULL;
}

