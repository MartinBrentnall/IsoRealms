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
#include "ResourceLayerBackground.h"

ResourceLayerBackground::ResourceLayerBackground(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Instance") {
      std::string mName = mNode->getAttribute("name");
      LayerBackground* mLayerBackgroundInstance = createInstance(mName);
      IArgumentValue* mArgumentValue = new ArgumentValueCustomType<LayerBackground>(mLayerBackgroundInstance);
      resourceRegistry->add(mArgumentValue, "Background", mName);
    }
  }
}

LayerBackground* ResourceLayerBackground::createInstance(const std::string& name) {
  LayerBackground* mNamedInstance = new LayerBackground(this);
  cNamedInstances[name] = mNamedInstance;
  return mNamedInstance;
} 

void ResourceLayerBackground::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
}

void ResourceLayerBackground::save(DOMNodeWriter* node, IResourceLocator* resources) {
  for (std::pair<std::string, LayerBackground*> mInstance : cNamedInstances) {
    DOMNodeWriter* mInstanceBranch = node->addBranch("Instance");
    mInstanceBranch->addAttribute("name", mInstance.first);
  }
}

// void ResourceLayerBackground::loadInstance(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
//   std::string mInstanceName = node->getAttribute("instance");
//   SpindizzyMap* mMapInstance = new SpindizzyMap(this, cEditing);
//   cNamedInstances[mInstanceName] = mMapInstance;
//   IArgumentSource* mArgumentSource = new ArgumentSourceCustom<SpindizzyMap>(mMapInstance);
//   runtimeContext->add(mMapInstance, mInstanceName, node);
//   runtimeContext->registerArgument("Map", mInstanceName, mArgumentSource);
// }

ILayer* ResourceLayerBackground::getLayer(DOMNodeWrapper* node, IResourceAccessor* resources, bool editing, bool asTemplate) {
  std::string mInstanceName = node->getAttribute("instance");
  return cNamedInstances[mInstanceName];
}

std::string ResourceLayerBackground::getInstanceName(ILayer* layer) {
  for (std::pair<std::string, LayerBackground*> mLayerInstance : cNamedInstances) {
    if (mLayerInstance.second == layer) {
      return mLayerInstance.first;
    }
  }
  return ""; // TODO: Throw
}

void ResourceLayerBackground::configureLayer() {
  // TODO: Implement this
}

bool ResourceLayerBackground::input(SDL_Event& event) {
  // TODO: Implement this
  return false;
}

void ResourceLayerBackground::renderEditingPreview() {
  // TODO: Implement this
}

void ResourceLayerBackground::renderIcon() {
  // TODO: Implement this
}

void ResourceLayerBackground::updateIcon(unsigned int) {
  // TODO: Implement this
}

void ResourceLayerBackground::destroy(ILayer* layer) {
  delete layer;
}
