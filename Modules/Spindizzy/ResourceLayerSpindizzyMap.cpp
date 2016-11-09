/*
 * Copyright 2015 Martin Brentnall
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
#include "ResourceLayerSpindizzyMap.h"

ResourceLayerSpindizzyMap::ResourceLayerSpindizzyMap(ISpindizzyMapModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cModuleInterface = module;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Instance") {
      std::string mName = mNode->getAttribute("name");
      LayerSpindizzyMap* mMapInstance = createInstance(mName);
      IArgumentValue* mArgumentValue = new ArgumentValueCustomType<LayerSpindizzyMap>(mMapInstance);
      resourceRegistry->add(mArgumentValue, "Map", mName);
    }
  }
}

void ResourceLayerSpindizzyMap::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
}

void ResourceLayerSpindizzyMap::save(DOMNodeWriter* node, IResourceLocator* resources) {
}

ISpindizzyMapModule* ResourceLayerSpindizzyMap::getSpindizzyMapInterface() {
  return cModuleInterface;
}

LayerSpindizzyMap* ResourceLayerSpindizzyMap::createInstance(const std::string& name) {
  LayerSpindizzyMap* mNamedInstance = new LayerSpindizzyMap(this);
  cNamedInstances[name] = mNamedInstance;
  return mNamedInstance;
} 

ILayer* ResourceLayerSpindizzyMap::getLayer(DOMNodeWrapper* node, IResourceAccessor* resources, bool editing, bool asTemplate) {
  std::string mInstanceName = node->getAttribute("instance");
  LayerSpindizzyMap* mMap = cNamedInstances[mInstanceName];
  mMap->load(node, editing, resources, asTemplate);
  return mMap;
}

void ResourceLayerSpindizzyMap::configureLayer() {
  // TODO: Implement this
}

bool ResourceLayerSpindizzyMap::input(SDL_Event& event) {
  // TODO: Implement this
  return false;
}

void ResourceLayerSpindizzyMap::renderEditingPreview() {
  // TODO: Implement this
}

void ResourceLayerSpindizzyMap::renderIcon() {
  // TODO: Implement this
}

void ResourceLayerSpindizzyMap::updateIcon(unsigned int) {
  // TODO: Implement this
}

void ResourceLayerSpindizzyMap::destroy(ILayer* layer) {
  delete layer;
}
