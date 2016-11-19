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
#include "ResourceElementSpindizzyEnemy.h"

ResourceElementSpindizzyEnemy::ResourceElementSpindizzyEnemy(ISpindizzyEnemySet* elementSet, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cModuleInterface = elementSet;
}

I3DModelType* ResourceElementSpindizzyEnemy::getModelType() {
  return cModelType;
}

void ResourceElementSpindizzyEnemy::setModelType(I3DModelType* modelType) {
  cSampleEnemy->setModelType(cModelType, modelType);
  for (unsigned int i = 0; i < cContent.size(); i++) {
    cContent[i]->setModelType(cModelType, modelType);
  }
  cModelType = modelType;
}

void ResourceElementSpindizzyEnemy::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mModelPath = node->getAttribute("model");
  cModelType = resourceAccessor->getModelType(mModelPath);
  BlockLocation mIdentityLocation(0, 0, 0);
  cSampleEnemy = new ElementSpindizzyEnemy(this, &mIdentityLocation, cModelType, nullptr);
}

void ResourceElementSpindizzyEnemy::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("model", resourceLocator->getPath(cModelType));
}

void ResourceElementSpindizzyEnemy::loadElement(DOMNodeWrapper* node, BlockLocation* relative, IElementContainer* container, IResourceAccessor* resources, bool asTemplate) {
  if (!asTemplate) {
    BlockLocation mLocation;
    mLocation.setRelative(node, *relative);
    ElementSpindizzyEnemy* mLoadedEnemy = new ElementSpindizzyEnemy(this, &mLocation, cModelType, container);
    cContent.push_back(mLoadedEnemy);
    ElementHandlerSpindizzyDynamic* mDynamicElementHandler = cModuleInterface->getDynamicElementHandler(container);
    mDynamicElementHandler->addElement(mLoadedEnemy);
  }
}

bool ResourceElementSpindizzyEnemy::keyDown(SDLKey& key, ILayerEditingContext* editingContext) {
  switch (key) {
    case SDLK_SPACE: {
      IElementContainer* mElementContainer = editingContext->getElementContainer();
      Vertex* mLocation = editingContext->getLocation();
      BlockLocation mGridLocation;
      mGridLocation.x = std::round(mLocation->x);
      mGridLocation.y = std::round(mLocation->y);
      mGridLocation.z = std::round(mLocation->z * 2.0);
      ElementSpindizzyEnemy* mEnemy = new ElementSpindizzyEnemy(this, &mGridLocation, cModelType, nullptr);
      ElementHandlerSpindizzyDynamic* mHandler = cModuleInterface->getDynamicElementHandler(mElementContainer);
      mHandler->addElement(mEnemy);
      mElementContainer->setDirty(mHandler);
      cContent.push_back(mEnemy);
      return true;
    }

    default: {
      return false;
    }
  }
  return false;
}

ElementSpindizzyEnemy* ResourceElementSpindizzyEnemy::getElement(IElement* element) {
  for (ElementSpindizzyEnemy* mElement : cContent) {
    if (mElement == element) {
      return mElement;
    }
  }
  return nullptr;
}

bool ResourceElementSpindizzyEnemy::inputEdit(SDL_Event& event, ILayerEditingContext* editingContext) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym, editingContext);
    }
  }
  return false;
}

void ResourceElementSpindizzyEnemy::configureElement() {
  // Nothing to do.
}

void ResourceElementSpindizzyEnemy::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
}

void ResourceElementSpindizzyEnemy::renderEditingPreview(Vertex& location) {
  glTranslatef(location.x, location.y, location.z);
  cSampleEnemy->renderRuntime();
}

void ResourceElementSpindizzyEnemy::updateEditingPreview(unsigned int milliseconds) {
  // TODO
}

void ResourceElementSpindizzyEnemy::updateIcon(unsigned int milliseconds) {
  // Nothing to do
}

void ResourceElementSpindizzyEnemy::renderIcon() {
  glTranslatef(0.0f, -1.0f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glScalef(2.0f, 2.0f, 2.0f);
  cSampleEnemy->renderStatic();
  if (cSampleEnemy->isVisualRuntime()) {
    cSampleEnemy->renderRuntime();
  }
}

void ResourceElementSpindizzyEnemy::destroy(IElement* element) {
  delete element;
}

void ResourceElementSpindizzyEnemy::removeElement(IElement* element) {
  ElementSpindizzyEnemy* mEnemy = getElement(element);
  IElementContainer* mContainer = mEnemy->getElementContainer();
  ElementHandlerSpindizzyDynamic* mItemHandler = cModuleInterface->getDynamicElementHandler(mContainer);
  mItemHandler->removeElement(mEnemy);
  if (mItemHandler->isEmpty()) {
    mContainer->removeElement(mItemHandler);
    cModuleInterface->removeElementHandlerSpindizzyDynamic(mContainer);
  }
}

Vertex* ResourceElementSpindizzyEnemy::editorCursorStopped(Vertex* location) {
  Vertex* mGridLocation = new Vertex();
  mGridLocation->x = std::round(location->x);
  mGridLocation->y = std::round(location->y);
  mGridLocation->z = std::round(location->z * 2.0) * 0.5;
  return mGridLocation;
}

ResourceElementSpindizzyEnemy::~ResourceElementSpindizzyEnemy() {
  delete cSampleEnemy;
  for (unsigned int i = 0; i < cContent.size(); i++) {
    removeElement(cContent[i]);
    destroy(cContent[i]);
  }
}
