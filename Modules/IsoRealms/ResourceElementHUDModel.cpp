/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#include "ResourceElementHUDModel.h"

ResourceElementHUDModel::ResourceElementHUDModel(IElementRelationManager* manager, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) : ResourceElementHUDAbstract(manager) {
}

IElement* ResourceElementHUDModel::createHUDElement(DOMNodeWrapper* node, DOMNodeWrapper* cache, BlockLocation* location, IResourceAccessor* resources, bool asTemplate, HUDComponentPosition* elementDimensions) {
  // TODO: Each Element should get its own Model
  return new ElementHUDModel(this, elementDimensions, cModel, cCamera);
}

void ResourceElementHUDModel::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  cModelLocation.x = node->getFloatAttribute("x");
  cModelLocation.y = node->getFloatAttribute("y");
  cModelLocation.z = node->getFloatAttribute("z");
  cModelScale = node->getFloatAttribute("scale");
  std::string mModelPath = node->getAttribute("model");
  std::string mCameraPath = node->getAttribute("camera");
  cModel = resources->getModel(mModelPath, &cModelLocation, cModelScale);
  cCamera = resources->getCamera(mCameraPath);
}

void ResourceElementHUDModel::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("model", resourceLocator->getPath(cModel));
  node->addAttribute("camera", resourceLocator->getPath(cCamera));
  node->addAttribute("scale", cModelScale);
  node->addAttribute("x", cModelLocation.x, 0.0f);
  node->addAttribute("y", cModelLocation.y, 0.0f);
  node->addAttribute("z", cModelLocation.z, 0.0f);
}

void ResourceElementHUDModel::saveCache(DOMNodeWriter* cache) {
  // Nothing to do
}

void ResourceElementHUDModel::configureElement() {
  // TODO
}

void ResourceElementHUDModel::destroy(IElement* element) {
  // TODO
}

void ResourceElementHUDModel::removeElement(IElement* element) {
  // TODO
}

Vertex* ResourceElementHUDModel::editorCursorStopped(Vertex* location) {
  return nullptr;
}

bool ResourceElementHUDModel::inputEdit(SDL_Event& event, ILayerEditingContext* editingContext) {
  return false;// TODO
}

void ResourceElementHUDModel::renderEditingPreview(Vertex& location) {
  // TODO
}

void ResourceElementHUDModel::updateEditingPreview(unsigned int milliseconds) {
  // TODO
}

void ResourceElementHUDModel::renderIcon() {
  // TODO
}

void ResourceElementHUDModel::updateIcon(unsigned int) {
  // TODO
}
