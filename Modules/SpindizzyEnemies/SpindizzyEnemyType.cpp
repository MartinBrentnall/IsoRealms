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
#include "SpindizzyEnemyType.h"

SpindizzyEnemyType::SpindizzyEnemyType(ISpindizzyEnemySet* elementSet) : ElementType<ISpindizzyEnemySet, SpindizzyEnemy>(elementSet) {
}

void SpindizzyEnemyType::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mModelPath = node->getAttribute("model");
  cModelType = resourceAccessor->getModelType(mModelPath);
  BlockLocation mIdentityLocation(0, 0, 0);
  cSampleEnemy = new SpindizzyEnemy(this, &mIdentityLocation, cModelType);
  cSampleEnemyVisuals = cSampleEnemy->getVisualElements();
}

void SpindizzyEnemyType::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("model", resourceLocator->getPath(cModelType));
}

IElement* SpindizzyEnemyType::getElement(DOMNodeWrapper* node, BlockLocation* relative, IElementContainer* container) {
  BlockLocation mLocation;
  mLocation.setRelative(node, *relative);
  SpindizzyEnemy* mLoadedEnemy = new SpindizzyEnemy(this, &mLocation, cModelType);
  cContent.push_back(mLoadedEnemy);
  ISpindizzyEnemySet* mEnemySet = getElementSet();
  mEnemySet->registerElement(container, mLoadedEnemy);
  return mLoadedEnemy;
}

IElementHandler* SpindizzyEnemyType::getElementHandler() {
  ISpindizzyEnemySet* mSpindizzyEnemySet = getElementSet();
  return mSpindizzyEnemySet->createHandler();
}

bool SpindizzyEnemyType::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      SpindizzyEnemy* mEnemy = new SpindizzyEnemy(this, cEditingLocation, cModelType);
      addElement(mEnemy);
      cContent.push_back(mEnemy);
      return true;
    }

    default: {
      return false;
    }
  }
  return false;
}

bool SpindizzyEnemyType::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void SpindizzyEnemyType::configureElement() {
  // Nothing to do.
}

void SpindizzyEnemyType::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
  cEditingLocation = editingLocation;  
}

void SpindizzyEnemyType::renderEditingPreview() {
  // Nothing to do
}

void SpindizzyEnemyType::updateIcon(int milliseconds) {
  // Nothing to do
}

void SpindizzyEnemyType::renderIcon() {
  glTranslatef(0.0f, -1.0f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glScalef(2.0f, 2.0f, 2.0f);
  cSampleEnemy->renderStatic();
  for (unsigned int i = 0; i < cSampleEnemyVisuals.size(); i++) {
    cSampleEnemyVisuals[i]->render();
  }
}

void SpindizzyEnemyType::destroy(IElement* element) {
  delete element;
}
