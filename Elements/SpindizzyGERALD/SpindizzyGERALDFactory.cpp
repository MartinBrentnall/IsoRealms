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
#include "SpindizzyGERALDFactory.h"

SpindizzyGERALDFactory::SpindizzyGERALDFactory(IElementSet* elementSet, ISimpleModelFactory* geraldModelFactory) : IElementFactory(elementSet) {
  cGERALDModelFactory = geraldModelFactory;
  BlockLocation mIdentityLocation(0, 0, 0);
  ISimpleModel* mSampleModel = cGERALDModelFactory->createModel();
  cSampleGERALD = new SpindizzyGERALD(this, &mIdentityLocation, mSampleModel);
  cSampleGERALDVisuals = cSampleGERALD->getVisualElements();
}

std::string SpindizzyGERALDFactory::getName() {
  return "";
}

void SpindizzyGERALDFactory::setModel(ISimpleModelFactory* modelFactory) {
  cGERALDModelFactory = modelFactory;
  ISimpleModel* mSampleModel = cGERALDModelFactory->createModel();
  cSampleGERALD->setModel(mSampleModel);
  for (unsigned int i = 0; i < cContent.size(); i++) {
    ISimpleModel* mModel = cGERALDModelFactory->createModel();
    cContent[i]->setModel(mModel);
  }
}

IElement* SpindizzyGERALDFactory::getElement(DOMNodeWrapper* node, BlockLocation* relative) {
  // TODO: Implement this!
  return NULL;
}

bool SpindizzyGERALDFactory::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      ISimpleModel* mModel = cGERALDModelFactory->createModel();
      SpindizzyGERALD* mGERALD = new SpindizzyGERALD(this, cEditingLocation, mModel);
      cGateway->pushElement(mGERALD);
      cContent.push_back(mGERALD);
      return true;
    }

    default: {
      return false;
    }
  }
  return false;
}

bool SpindizzyGERALDFactory::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void SpindizzyGERALDFactory::configureElement() {
  // Nothing to do.
}

void SpindizzyGERALDFactory::setEditingInfo(BlockLocation* editingLocation, IElementGateway* gateway, IComponentContainer* componentContainer) {
  cGateway = gateway;
  cEditingLocation = editingLocation;  
}

void SpindizzyGERALDFactory::renderEditingPreview() {
  // Nothing to do
}

void SpindizzyGERALDFactory::updateIcon(int milliseconds) {
  // Nothing to do
}

void SpindizzyGERALDFactory::renderIcon() {
  glTranslatef(0.0f, -1.0f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glScalef(2.0f, 2.0f, 2.0f);
  cSampleGERALD->renderStatic();
  for (unsigned int i = 0; i < cSampleGERALDVisuals.size(); i++) {
    cSampleGERALDVisuals[i]->render();
  }
}

