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

Map::Map(IProject* project) {
  cProject = project;
  cCamera = NULL;
  cZoneHandlers.push_back(new DefaultZoneHandler());
}

Map::Map(bool editing, IProject* project, IResourceAccessor* resources) {
  cProject = project;
  if (editing) {
    cZoneHandlers.push_back(new DefaultZoneHandler());
  }
  cEditing = editing;
}

Map::Map(DOMNodeWrapper* node, bool editing, IProject* project, IResourceAccessor* resources) {
  cProject = project;
  if (editing) {
    cZoneHandlers.push_back(new DefaultZoneHandler());
  }
  cEditing = editing;
  initialiseResource(node, resources);
}

void Map::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  BlockLocation mStartLocation(0, 0, 0);
  std::string mCameraPath = node->getAttribute("camera");
  cCamera = resources->getCamera(mCameraPath);
  
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Element") {
      resources->loadElement(mNode, &mStartLocation, this);
    } else if (mValueAsString == "ZoneHandler") {
      std::string mZoneHandlerName = mNode->getAttribute("value");
      IZoneHandler* mZoneRenderer = resources->getZoneHandler(mZoneHandlerName);
      if (!cEditing) {
        cZoneHandlers.push_back(mZoneRenderer);
      } else {
        cZoneHandlersPersisted.push_back(mZoneRenderer);
      }
    } else if (mValueAsString == "Zone") {
      Zone* mZone = new Zone(mNode, cProject, resources, this);
      addZone(mZone);
    } else {
      // TODO: Throw something
    }
  }
  cElementHandler.setAllDirty();
}

void Map::addElement(IElement* element) {
  cElementHandler.addElement(element);
}

DOMNodeWrapper* Map::getConfigurationNode(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "InputConfiguration") {
      return mNode;
    }
  }
  return NULL;
}

void Map::addZone(Zone* zone) {
  cDirtyZones.push_back(zone);
  cZones.push_back(zone);
  zone->addChangeListener(this);
}

void Map::removeZone(Zone* zone) {
  for (unsigned int i = 0; i < cZones.size(); i++) {
    if (cZones[i] == zone) {
      cZones.erase(cZones.begin() + i);
      for (unsigned int j = 0; j < cDirtyZones.size(); j++) {
        if (cDirtyZones[j] == zone) {
          cDirtyZones.erase(cDirtyZones.begin() + j);
          break;
        }
      }
      break;
    }
  }
  delete zone;
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

IZone* Map::getZone(Vertex& location) {
  for (unsigned int i = 0; i < cZones.size(); i++) {
    if (cZones[i]->contains(location)) {
      return cZones[i];
    }
  }
  return NULL;
}

std::vector<ZoneEvent*> Map::getZoneEvents(Vertex& start, Vertex& end) {
  std::vector<ZoneEvent*> mAllZoneEvents;
  // TODO: Divide map into abstract segments using a B-tree in order to do more efficient searching
  for (unsigned int i = 0; i < cZones.size(); i++) {
    std::vector<ZoneEvent*> mZoneEvents = cZones[i]->getZoneEvents(start, end);
    for (unsigned int j = 0; j < mZoneEvents.size(); j++) {
      bool mInserted = false;
      for (unsigned int k = 0; k < mAllZoneEvents.size(); k++) {
        if (*mZoneEvents[j] < *mAllZoneEvents[k]) {
          mAllZoneEvents.insert(mAllZoneEvents.begin() + k, 1, mZoneEvents[j]);
          mInserted = true;
          break;
        }
      }
      if (!mInserted) {
        mAllZoneEvents.push_back(mZoneEvents[j]);
      }
    }
  }
  return mAllZoneEvents;
}

std::vector<IZone*> Map::getAdjacentZones(IZone* zone) {
  std::vector<IZone*> mAdjacentZones;
  for (unsigned int i = 0; i < cZones.size(); i++) {
    if (cZones[i]->isAdjacent(*(dynamic_cast<Zone*>(zone)))) { // TODO: THIS DYNAMIC CAST DEFINITELY SHOULD NOT BE HERE
      mAdjacentZones.push_back(cZones[i]);
    }
  }
  return mAdjacentZones;
}

bool Map::overlaps(BlockArea& blockArea) {
  for (unsigned int i = 0; i < cZones.size(); i++) {
    if (cZones[i]->overlaps(blockArea)) {
      return true;
    }
  }
  return false;
} 

int Map::getZoneIndex(IZone* zone) {
  for (unsigned int i = 0; i < cDirtyZones.size(); i++) {
    if (cDirtyZones[i] == zone) {
      return i;
    }
  }
  // TODO: Throw exception
  return -1;
}

void Map::initMap(unsigned int pass, bool editing) {
  std::vector<IZone*> mCleanZones;
  for (unsigned int i = 0; i < cDirtyZones.size(); i++) {
    cProject->initPlugins(cDirtyZones[i], pass);
    cProject->renderPreZone(cDirtyZones[i]);
    if (cDirtyZones[i]->initZone(pass, editing)) {
      mCleanZones.push_back(cDirtyZones[i]);
    }
  }
  
  for (unsigned int i = 0; i < mCleanZones.size(); i++) {
    int mIndexToRemove = getZoneIndex(mCleanZones[i]);
    cDirtyZones.erase(cDirtyZones.begin() + mIndexToRemove);
  }
  
  cElementHandler.init(pass, editing);
}

void Map::initRuntime() {
  initMap(false);
}

void Map::input(SDL_Event& event) {
  for (unsigned int i = 0; i < cZones.size(); i++) {
    cZones[i]->input(event);
  }
//  cElementHandler.input(event);
}

void Map::initMap(bool editing) {
  unsigned int mInitPass = 0;
  while (!cDirtyZones.empty() || cElementHandler.isDirty()) {
    initMap(mInitPass, editing);
    mInitPass++;
  }
}

void Map::zoneChanged(IZone* zone) {
  // TODO: More efficient way of checking if the zone is already dirty
  for (unsigned int i = 0; i < cDirtyZones.size(); i++) {
    if (cDirtyZones[i] == zone) {
      return;
    }
  }
  cDirtyZones.push_back(zone);
}

void Map::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& location) {
  DOMNodeWriter* mMapNode = node->addBranch("Map");
  for (unsigned int i = 0; i < cZoneHandlersPersisted.size(); i++) {
    DOMNodeWriter* mZoneHandlerNode = mMapNode->addBranch("ZoneHandler");
    std::string mZoneHandlerPath = resourceLocator->getPath(cZoneHandlersPersisted[i]);
    mZoneHandlerNode->addAttribute("value", mZoneHandlerPath);
  }
  BlockLocation mStartLocation(0, 0, 0);
  cElementHandler.save(mMapNode, resourceLocator, mStartLocation);
  for (unsigned int i = 0; i < cZones.size(); i++) {
    DOMNodeWriter* mZoneNode = mMapNode->addBranch("Zone");
    DOMNodeWriter* mPluginsNode = mZoneNode->addBranch("Plugins");
    cProject->savePluginData(mPluginsNode, this, cZones[i]);
    cZones[i]->save(mZoneNode, resourceLocator);
  }
}

void Map::pushElement(IElement* element) {
  cElementHandler.addElement(element);
}

Zone* Map::getElementContainer(IElement* element) {
  for (unsigned int i = 0; i < cZones.size(); i++) {
    if (cZones[i]->contains(element)) {
      return cZones[i];
    }
  }
  std::cout << "Warning: Element for removal not found in any zone.  Segmentation fault may follow!" << std::endl;
  return NULL;
}

void Map::setZoneHandler(IZoneHandler* zoneHandler) {
  cZoneHandlers.clear();
  cZoneHandlers.push_back(zoneHandler);
}

void Map::staticChanged() {
  for (unsigned int i = 0; i < cZones.size(); i++) {
    cZones[i]->staticChanged();
  }
}

void Map::removeElement(IElement* element) {
//   for (unsigned int i = 0; i < cZones.size(); i++) {
//     if (cZones[i]->removeElement(element)) {
//       zoneChanged(cZones[i]);
//       return cZones[i];
//     }
//   }
//   std::cout << "Warning: Element for removal not found in any zone.  Segmentation fault may follow!" << std::endl;
//   return NULL;
}

bool Map::containsElement(IElement* element) {
  return cElementHandler.contains(element);
}

float Map::getEast() {
  int mValue = INT_MIN;
  for (unsigned int i = 0; i < cZones.size(); i++) {
    mValue = std::max(cZones[i]->getEast(), mValue);
  }
  return mValue;
}

float Map::getWest() {
  int mValue = INT_MAX;
  for (unsigned int i = 0; i < cZones.size(); i++) {
    mValue = std::min(cZones[i]->getWest(), mValue);
  }
  return mValue;
}

float Map::getNorth() {
  int mValue = INT_MIN;
  for (unsigned int i = 0; i < cZones.size(); i++) {
    mValue = std::max(cZones[i]->getNorth(), mValue);
  }
  return mValue;
}

float Map::getSouth() {
  int mValue = INT_MAX;
  for (unsigned int i = 0; i < cZones.size(); i++) {
    mValue = std::min(cZones[i]->getSouth(), mValue);
  }
  return mValue;
}

float Map::getTop() {
  int mValue = INT_MIN;
  for (unsigned int i = 0; i < cZones.size(); i++) {
    mValue = std::max(cZones[i]->getTop(), mValue);
  }
  return mValue;
}

float Map::getBottom() {
  int mValue = INT_MAX;
  for (unsigned int i = 0; i < cZones.size(); i++) {
    mValue = std::min(cZones[i]->getBottom(), mValue);
  }
  return mValue;
}

float Map::getAspectRatio() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  return mScreenConfiguration->getAspectRatio();
}

int Map::getZoneCount() {
  return cZones.size();
}

IPlugin* Map::getElementSet() {
  return NULL;
}

IElementType* Map::getElementType() {
  return NULL;
}

bool Map::initElement(unsigned int pass) {
  initMap(false);
  return true;
}

bool Map::isDynamicEditing() {
  return true;
}

bool Map::isDynamicRuntime() {
  return true;
}

bool Map::isInteractive() {
  return true;
}

bool Map::isVisualEditing() {
  return true;
}

bool Map::isVisualRuntime() {
  return true;
}

void Map::updateEditing(unsigned int milliseconds) {
  initMap(true);
  // TODO: Need a more permanent solution for better performance.
  std::vector<IZone*> mZones;
  for (unsigned int i = 0; i < cZones.size(); i++) {
    mZones.push_back(cZones[i]);
  }
  // TODO: End.

  for (unsigned int i = 0; i < cZoneHandlers.size(); i++) {
    cZoneHandlers[i]->updateEditing(mZones, milliseconds);
  }
  cElementHandler.updateEditing(milliseconds);
}

void Map::updateRuntime(unsigned int milliseconds) {
  cCamera->update(milliseconds);
  // TODO: Need a more permanent solution for better performance.
  std::vector<IZone*> mZones;
  for (unsigned int i = 0; i < cZones.size(); i++) {
    mZones.push_back(cZones[i]);
  }
  // TODO: End.
  
  for (unsigned int i = 0; i < cZoneHandlers.size(); i++) {
    cZoneHandlers[i]->updateRuntime(mZones, milliseconds);
  }
  cElementHandler.updateRuntime(milliseconds);
}

void Map::renderStatic() {
  cElementHandler.renderStatic();
}

void Map::renderRuntime() {
  cCamera->render();
  glColor3f(1.0f, 1.0f, 1.0f);
  // TODO: Need a more permanent solution for better performance.
  std::vector<IZone*> mZones;
  for (unsigned int i = 0; i < cZones.size(); i++) {
    mZones.push_back(cZones[i]);
  }
  // TODO: End.
  for (unsigned int i = 0; i < cZoneHandlers.size(); i++) {
    cZoneHandlers[i]->render(mZones, cProject);
  }
  cElementHandler.renderRuntime();
}

void Map::renderEditing() {
  cCamera->render();
  glColor3f(1.0f, 1.0f, 1.0f);
  // TODO: Need a more permanent solution for better performance.
  std::vector<IZone*> mZones;
  for (unsigned int i = 0; i < cZones.size(); i++) {
    mZones.push_back(cZones[i]);
  }
  // TODO: End.
  for (unsigned int i = 0; i < cZoneHandlers.size(); i++) {
    cZoneHandlers[i]->render(mZones, cProject);
  }
  cElementHandler.renderEditing();
}

void Map::setDirty() {
  // TODO: Implement this
}

Map::~Map() {
  for (unsigned int i = 0; i < cZones.size(); i++) {
    delete cZones[i];
  }
  cZones.clear();
}
