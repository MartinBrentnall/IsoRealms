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
#include "Map.h"

Map::Map() {
}

Map::Map(DOMNodeWrapper* node, IPluginRegistryListener* pluginRegistryListener, IElementRegistryListener* elementRegistryListener) {
  cPluginRegistry.addListener(pluginRegistryListener);
  cElementSetRegistry.addElementRegistryListener(elementRegistryListener);
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Plugin") {
      cPluginRegistry.registerPlugin(mNode);
    } else if (mValueAsString == "ElementSet") {
      cElementSetRegistry.registerElementSet(&cPluginRegistry, mNode);
    } else if (mValueAsString == "Zone") {
      Zone* mZone = new Zone(mNode, cElementSetRegistry);
      addZone(mZone);
    } else {
      // TODO: Throw something
    }
  }
}

void Map::addZone(Zone* zone) {
  cDirtyZones.push_back(zone);
  cZones.push_back(zone);
  zone->addChangeListener(this);
}

Zone* Map::getZone(BlockLocation& location) {
  // TODO: Divide map into abstract segments using a B-tree in order to do more efficient searching
  for (unsigned int i = 0; i < cZones.size(); i++) {
    if (cZones[i]->contains(location)) {
      return cZones[i];
    }
  }
  return NULL;
}

bool Map::overlaps(BlockArea& blockArea) {
  for (unsigned int i = 0; i < cZones.size(); i++) {
    if (cZones[i]->overlaps(blockArea)) {
      return true;
    }
  }
  return false;
} 

int Map::getZoneIndex(Zone* zone) {
  for (unsigned int i = 0; i < cDirtyZones.size(); i++) {
    if (cDirtyZones[i] == zone) {
      return i;
    }
  }
  // TODO: Throw exception
  return -1;
}

void Map::notifyZoneAction(Zone* zone) {
  cPluginRegistry.notifyZoneAction(zone);
}

void Map::initMap() {
  std::vector<Zone*> mCleanZones;
  for (unsigned int i = 0; i < cDirtyZones.size(); i++) {
    cPluginRegistry.initPlugins(cDirtyZones[i]);
    if (cDirtyZones[i]->initZone()) {
      mCleanZones.push_back(cDirtyZones[i]);
    }
  }
  for (unsigned int i = 0; i < mCleanZones.size(); i++) {
    int mIndexToRemove = getZoneIndex(mCleanZones[i]);
    cDirtyZones.erase(cDirtyZones.begin() + mIndexToRemove);
  }
  mCleanZones.clear();
}

void Map::update(int milliseconds) {
  while (!cDirtyZones.empty()) {
    initMap();
  }
  for (unsigned int i = 0; i < cZones.size(); i++) {
    cZones[i]->update(milliseconds);
  }
}

void Map::render() {
  for (unsigned int i = 0; i < cZones.size(); i++) {
    cZones[i]->render();
  }
}

void Map::zoneChanged(Zone* zone) {
  // TODO: More efficient way of checking if the zone is already dirty
  for (unsigned int i = 0; i < cDirtyZones.size(); i++) {
    if (cDirtyZones[i] == zone) {
      return;
    }
  }
  cDirtyZones.push_back(zone);
}

void Map::save() {
  DOMNodeWriter* mMapNode = new DOMNodeWriter("Map");
  cPluginRegistry.save(mMapNode);
  cElementSetRegistry.save(&cPluginRegistry, mMapNode);
  for (unsigned int i = 0; i < cZones.size(); i++) {
    cZones[i]->save(&cElementSetRegistry, mMapNode);
  }
  mMapNode->save("Test.isorealms");
}

void Map::removeElement(IElement* element) {
  for (unsigned int i = 0; i < cZones.size(); i++) {
    if (cZones[i]->removeElement(element)) {
      zoneChanged(cZones[i]);
    }
  }
}

ElementSetRegistry* Map::getElementSetRegistry() {
  return &cElementSetRegistry;
}

PluginRegistry* Map::getPluginRegistry() {
  return &cPluginRegistry;
}

void Map::pluginInstanceAdded(PluginRegistry* registry, std::string, std::string) {
  // Nothing to do.
}

void Map::pluginInstanceRemoved(IPlugin* instance, std::string type) {
  cPluginRegistry.pluginRemoved(instance);
  cElementSetRegistry.pluginRemoved(instance);
}

Map::~Map() {
  for (unsigned int i = 0; i < cZones.size(); i++) {
    delete cZones[i];
  }
  cZones.clear();
}
