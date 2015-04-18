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
#include "LayerSpindizzyMap.h"

LayerSpindizzyMap::LayerSpindizzyMap(ISpindizzyMapType* type) {
  cMapType = type;
  cCamera = NULL;
  cElementHandler.setMultiThreaded(true);
}

void LayerSpindizzyMap::load(DOMNodeWrapper* node, bool editing, IResourceAccessor* resources) {
  cEditing = editing;
  initialiseResource(node, resources);
  if (cEditing) {
    Vertex mNormalDistance(0.0f, 0.0f, -20.0f);
    cCameraEditing.addViewPoint(0, mNormalDistance, 315.0f, -50.0f, 0.0f);
    cCameraEditing.setViewPoint(0);
  }
}

void LayerSpindizzyMap::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  BlockLocation mStartLocation(0, 0, 0);
  std::string mCameraPath = node->getAttribute("camera");
  cCamera = resources->getCamera(mCameraPath);
  
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Element") {
      resources->loadElement(mNode, &mStartLocation, this);
    } else {
      // TODO: Throw something
    }
  }
  cElementHandler.setAllDirty();
}

void LayerSpindizzyMap::addElement(IElement* element) {
  cElementHandler.addElement(element);
}

DOMNodeWrapper* LayerSpindizzyMap::getConfigurationNode(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "InputConfiguration") {
      return mNode;
    }
  }
  return NULL;
}

void LayerSpindizzyMap::initRuntime() {
  cElementHandler.init(0, cEditing);
}

void LayerSpindizzyMap::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  DOMNodeWriter* mMapNode = node->addBranch("Map");
  BlockLocation mStartLocation(0, 0, 0);
  cElementHandler.save(mMapNode, resourceLocator, mStartLocation);
}

void LayerSpindizzyMap::pushElement(IElement* element) {
  cElementHandler.addElement(element);
}

void LayerSpindizzyMap::staticChanged() {
//   for (unsigned int i = 0; i < cZones.size(); i++) {
//     cZones[i]->staticChanged();
//   }
  // TODO  
}

void LayerSpindizzyMap::removeElement(IElement* element) {
//   for (unsigned int i = 0; i < cZones.size(); i++) {
//     if (cZones[i]->removeElement(element)) {
//       zoneChanged(cZones[i]);
//       return cZones[i];
//     }
//   }
//   std::cout << "Warning: Element for removal not found in any zone.  Segmentation fault may follow!" << std::endl;
//   return NULL;
}

void LayerSpindizzyMap::addArgumentValue(IArgument* argument) {
  // TODO?
}

void LayerSpindizzyMap::setArguments() {
  // TODO?
}

void LayerSpindizzyMap::unsetArguments() {
  // TODO?
}

BlockArea* LayerSpindizzyMap::getCoverage() {
  return nullptr;
}

bool LayerSpindizzyMap::containsElement(IElement* element) {
  return cElementHandler.contains(element);
}

float LayerSpindizzyMap::getEast() {
  return cElementHandler.getStaticBounds()->getEast();
}

float LayerSpindizzyMap::getWest() {
  return cElementHandler.getStaticBounds()->getWest();
}

float LayerSpindizzyMap::getNorth() {
  return cElementHandler.getStaticBounds()->getNorth();
}

float LayerSpindizzyMap::getSouth() {
  return cElementHandler.getStaticBounds()->getSouth();
}

float LayerSpindizzyMap::getTop() {
  return cElementHandler.getStaticBounds()->getTop();
}

float LayerSpindizzyMap::getBottom() {
  return cElementHandler.getStaticBounds()->getBottom();
}

float LayerSpindizzyMap::getAspectRatio() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  return mScreenConfiguration->getAspectRatio();
}

ILayerType* LayerSpindizzyMap::getLayerType() {
  return cMapType;
}

void LayerSpindizzyMap::updateEditing(unsigned int milliseconds) {
  cElementHandler.init(0, cEditing);
  cCameraEditing.update(milliseconds);
  // TODO: Need a more permanent solution for better performance.
//   std::vector<IZone*> mZones;
//   for (unsigned int i = 0; i < cZones.size(); i++) {
//     mZones.push_back(cZones[i]);
//   }
  // TODO: End.
  cElementHandler.updateEditing(milliseconds);
}

void LayerSpindizzyMap::updateRuntime(unsigned int milliseconds) {
  cCamera->update(milliseconds);
  // TODO: Need a more permanent solution for better performance.
//   std::vector<IZone*> mZones;
//   for (unsigned int i = 0; i < cZones.size(); i++) {
//     mZones.push_back(cZones[i]);
//   }
  // TODO: End.
  cElementHandler.updateRuntime(milliseconds);
}

void LayerSpindizzyMap::renderRuntime() {
  cCamera->render();
  glColor3f(1.0f, 1.0f, 1.0f);
  // TODO: Need a more permanent solution for better performance.
  // TODO: End.
  cElementHandler.renderRuntime();
  cElementHandler.renderStatic();
}

void LayerSpindizzyMap::renderEditing() {
  cCameraEditing.render();
  glColor3f(1.0f, 1.0f, 1.0f);
  // TODO: Need a more permanent solution for better performance.
//   std::vector<IZone*> mZones;
//   for (unsigned int i = 0; i < cZones.size(); i++) {
//     mZones.push_back(cZones[i]);
//   }
  // TODO: End.
  cElementHandler.renderEditing();
  cElementHandler.renderStatic();
}

void LayerSpindizzyMap::input(SDL_Event& event) {
  if (cCameraEditing.input(event)) {
    return;
  }
}

LayerSpindizzyMap::~LayerSpindizzyMap() {
}
