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
#include "SpindizzyEnemyFactory.h"

SpindizzyEnemyFactory::SpindizzyEnemyFactory(ISpindizzyEnemySet* elementSet, const std::string& modelPath, const std::string& type, IRuntimeContext* runtimeContext) : ElementFactory<ISpindizzyEnemySet, SpindizzyEnemy>(elementSet) {
  cRuntimeContext = runtimeContext;
  cType = type;
  cModelPath = modelPath;
  BlockLocation mIdentityLocation(0, 0, 0);
  cSampleEnemy = new SpindizzyEnemy(this, &mIdentityLocation, cModelPath, cRuntimeContext);
  cSampleEnemyVisuals = cSampleEnemy->getVisualElements();
}

IElement* SpindizzyEnemyFactory::getElement(DOMNodeWrapper* node, BlockLocation* relative, IElementContainer* elementContainer) {
  BlockLocation mLocation;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Location") {
      mLocation.setRelative(mNode, *relative);
    }
  }
  SpindizzyEnemy* mLoadedEnemy = new SpindizzyEnemy(this, &mLocation, cModelPath, cRuntimeContext);
  cContent.push_back(mLoadedEnemy);
  return mLoadedEnemy;
}

bool SpindizzyEnemyFactory::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      SpindizzyEnemy* mEnemy = new SpindizzyEnemy(this, cEditingLocation, cModelPath, cRuntimeContext);
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

bool SpindizzyEnemyFactory::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void SpindizzyEnemyFactory::configureElement() {
  // Nothing to do.
}

void SpindizzyEnemyFactory::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
  cEditingLocation = editingLocation;  
}

void SpindizzyEnemyFactory::renderEditingPreview() {
  // Nothing to do
}

void SpindizzyEnemyFactory::updateIcon(int milliseconds) {
  // Nothing to do
}

void SpindizzyEnemyFactory::renderIcon() {
  glTranslatef(0.0f, -1.0f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glScalef(2.0f, 2.0f, 2.0f);
  cSampleEnemy->renderStatic();
  for (unsigned int i = 0; i < cSampleEnemyVisuals.size(); i++) {
    cSampleEnemyVisuals[i]->render();
  }
}

std::string SpindizzyEnemyFactory::getName() {
  return cType;
}

