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
#include "ResourceElementSpindizzyItem.h"

ResourceElementSpindizzyItem::ResourceElementSpindizzyItem(ISpindizzyJewelSet* elementSet, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cModuleInterface = elementSet;
  cBoundaries = new Boundaries();
  resourceRegistry->add(cBoundaries, node->getAttribute("name"));
}

I3DModelType* ResourceElementSpindizzyItem::getModelType() {
  return cModelType;
}

void ResourceElementSpindizzyItem::setModelType(I3DModelType* modelType) {
  cSampleJewel->setModelType(cModelType, modelType);
  for (unsigned int i = 0; i < cContent.size(); i++) {
    cContent[i]->setModelType(cModelType, modelType);
  }
  cModelType = modelType;
}

void ResourceElementSpindizzyItem::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mModelPath = node->getAttribute("model");
  cModelType = resourceAccessor->getModelType(mModelPath);
  BlockLocation mIdentityLocation(0, 0, 0);
  cSampleJewel = new ElementSpindizzyItem(this, &mIdentityLocation, cModelType, nullptr);
  cEditingJewel = new ElementSpindizzyItem(this, &mIdentityLocation, cModelType, nullptr);
}

void ResourceElementSpindizzyItem::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("model", resourceLocator->getPath(cModelType));
}

ISpindizzyJewelSet* ResourceElementSpindizzyItem::getSpindizzyItemInterface() {
  return cModuleInterface;
}

void ResourceElementSpindizzyItem::loadElement(DOMNodeWrapper* node, BlockLocation* relative, IElementContainer* container, IResourceAccessor* resources) {
  BlockLocation* mLocation = new BlockLocation();
  mLocation->setRelative(node, *relative);
  ElementSpindizzyItem* mJewel = new ElementSpindizzyItem(this, mLocation, cModelType, container);
  cContent.push_back(mJewel);
  ElementHandlerItem* mDynamicElementHandler = cModuleInterface->getItemElementHandler(container);
  mDynamicElementHandler->addElement(mJewel);
}

bool ResourceElementSpindizzyItem::keyDown(SDLKey& key, ILayerEditingContext* editingContext) {
  switch (key) {
    case SDLK_SPACE: {
      IElementContainer* mElementContainer = editingContext->getElementContainer();
      Vertex* mLocation = editingContext->getLocation();
      BlockLocation mGridLocation;
      mGridLocation.x = std::round(mLocation->x);
      mGridLocation.y = std::round(mLocation->y);
      mGridLocation.z = std::round(mLocation->z * 2.0);
      ElementSpindizzyItem* mJewel = new ElementSpindizzyItem(this, &mGridLocation, cModelType, nullptr);
      mElementContainer->addElement(mJewel);
      cContent.push_back(mJewel);
      return true;
    }

    default: {
      return false;
    }
  }
  return false;
}

bool ResourceElementSpindizzyItem::inputEdit(SDL_Event& event, ILayerEditingContext* editingContext) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym, editingContext);
    }
  }
  return false;
}

void ResourceElementSpindizzyItem::configureElement() {
  // Nothing to do.
}

void ResourceElementSpindizzyItem::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
}

void ResourceElementSpindizzyItem::renderEditingPreview() {
  cEditingJewel->renderRuntime();
}

void ResourceElementSpindizzyItem::updateIcon(unsigned int milliseconds) {
  if (cSampleJewel->isDynamicRuntime()) {
    cSampleJewel->updateRuntime(milliseconds);
  }
}

void ResourceElementSpindizzyItem::setDirty(IElement* element) {
  // Nothing to do for this element
}

void ResourceElementSpindizzyItem::renderIcon() {
  glTranslatef(0.0f, -0.6f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glScalef(2.0f, 2.0f, 2.0f);
  cSampleJewel->renderStatic();
  if (cSampleJewel->isVisualRuntime()) {
    cSampleJewel->renderRuntime();
  }
}

IBoundaries* ResourceElementSpindizzyItem::getCollectables() {
  return cBoundaries;
}

void ResourceElementSpindizzyItem::destroy(IElement* jewel) {
  delete jewel;
}

Vertex* ResourceElementSpindizzyItem::editorCursorStopped(Vertex* location) {
  Vertex* mGridLocation = new Vertex();
  mGridLocation->x = std::round(location->x);
  mGridLocation->y = std::round(location->y);
  mGridLocation->z = std::round(location->z * 2.0) * 0.5;
  return mGridLocation;
}

ResourceElementSpindizzyItem::~ResourceElementSpindizzyItem() {
  delete cSampleJewel;
  for (unsigned int i = 0; i < cContent.size(); i++) {
    IElementContainer* mContainer = cContent[i]->getElementContainer();
    ElementHandlerItem* mHandler = cModuleInterface->getItemElementHandler(mContainer);
    mHandler->removeElement(cContent[i]);
    if (mHandler->isEmpty()) {
      mContainer->removeElement(mHandler);
      cModuleInterface->removeElementHandlerItem(mContainer);
    }
    delete cContent[i];
  }
}

