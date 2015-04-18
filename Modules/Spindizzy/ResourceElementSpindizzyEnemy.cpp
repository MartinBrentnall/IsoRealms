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

void ResourceElementSpindizzyEnemy::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mModelPath = node->getAttribute("model");
  cModelType = resourceAccessor->getModelType(mModelPath);
  BlockLocation mIdentityLocation(0, 0, 0);
  cSampleEnemy = new ElementSpindizzyEnemy(this, &mIdentityLocation, cModelType);
}

void ResourceElementSpindizzyEnemy::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("model", resourceLocator->getPath(cModelType));
}

void ResourceElementSpindizzyEnemy::loadElement(DOMNodeWrapper* node, BlockLocation* relative, IElementContainer* container, IResourceAccessor* resources) {
  BlockLocation mLocation;
  mLocation.setRelative(node, *relative);
  ElementSpindizzyEnemy* mLoadedEnemy = new ElementSpindizzyEnemy(this, &mLocation, cModelType);
  cContent.push_back(mLoadedEnemy);
  ElementHandlerSpindizzyDynamic* mDynamicElementHandler = cModuleInterface->getDynamicElementHandler(container);
  mDynamicElementHandler->addElement(mLoadedEnemy);
}

bool ResourceElementSpindizzyEnemy::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      ElementSpindizzyEnemy* mEnemy = new ElementSpindizzyEnemy(this, cEditingLocation, cModelType);
//      addElement(mEnemy);
      cContent.push_back(mEnemy);
      return true;
    }

    default: {
      return false;
    }
  }
  return false;
}

bool ResourceElementSpindizzyEnemy::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void ResourceElementSpindizzyEnemy::configureElement() {
  // Nothing to do.
}

void ResourceElementSpindizzyEnemy::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
  cEditingLocation = editingLocation;  
}

void ResourceElementSpindizzyEnemy::renderEditingPreview() {
  // Nothing to do
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
