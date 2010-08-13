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

SpindizzyGERALDFactory::SpindizzyGERALDFactory(ISpindizzyGERALDSet* elementSet, ISimpleModelFactory* geraldModelFactory, ILocationAwareness* locationAwareness, IZoneContext* zoneContext) : ElementFactory<ISpindizzyGERALDSet>(elementSet) {
  cLocationAwareness = locationAwareness;
  cZoneContext = zoneContext;
  cGERALDModelFactory = geraldModelFactory;
  BlockLocation mIdentityLocation(0, 0, 0);
  cSampleGERALD = new SpindizzyGERALD(this, &mIdentityLocation, cGERALDModelFactory, NULL, NULL, NULL, NULL, NULL, 0.0f, NULL);
  cSampleGERALDVisuals = cSampleGERALD->getVisualElements();
  cCamera = NULL;
}

std::string SpindizzyGERALDFactory::getName() {
  return "";
}

void SpindizzyGERALDFactory::setModel(ISimpleModelFactory* modelFactory) {
  cGERALDModelFactory = modelFactory;
  cSampleGERALD->setModel(cGERALDModelFactory);
  for (unsigned int i = 0; i < cContent.size(); i++) {
    cContent[i]->setModel(cGERALDModelFactory);
  }
}

void SpindizzyGERALDFactory::setCamera(ICamera* camera) {
  cCamera = camera;
  for (unsigned int i = 0; i < cContent.size(); i++) {
    cContent[i]->setCamera(camera);
  }
}

void SpindizzyGERALDFactory::setCollectables(ICollectables* collectables) {
  cCollectables = collectables;
  for (unsigned int i = 0; i < cContent.size(); i++) {
    cContent[i]->setCollectables(collectables);
  }
}

void SpindizzyGERALDFactory::setCollidableSurfaceRegistry(ICollidableSurfaceRegistry* collidableSurfaceRegistry) {
  cCollidableSurfaceRegistry = collidableSurfaceRegistry;
}

void SpindizzyGERALDFactory::setLocationAwareness(ILocationAwareness* locationAwareness) {
  cLocationAwareness = locationAwareness;
}

void SpindizzyGERALDFactory::setZoneContext(IZoneContext* zoneContext) {
  cZoneContext = zoneContext;
  for (unsigned int i = 0; i < cContent.size(); i++) {
    cContent[i]->setZoneContext(zoneContext);
  }
}

IElement* SpindizzyGERALDFactory::getElement(DOMNodeWrapper* node, BlockLocation* relative, IElementContainer* elementContainer) {
  BlockLocation mStartLocation;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Location") {
      mStartLocation.setRelative(mNode, *relative);
    }
  }
  SpindizzyGERALD* mLoadedGERALD = new SpindizzyGERALD(this, &mStartLocation, cGERALDModelFactory, cCollectables, cCollidableSurfaceRegistry, cLocationAwareness, cZoneContext, cCamera, cFallLimit, cFallLimitScript);
  cContent.push_back(mLoadedGERALD);
  registerElement(mLoadedGERALD, elementContainer);
  return mLoadedGERALD;
}

bool SpindizzyGERALDFactory::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      if (cContent.size() == 0) {
        SpindizzyGERALD* mGERALD = new SpindizzyGERALD(this, cEditingLocation, cGERALDModelFactory, cCollectables, cCollidableSurfaceRegistry, cLocationAwareness, cZoneContext, cCamera, cFallLimit, cFallLimitScript);
        addElement(mGERALD);
        cContent.push_back(mGERALD);
      } else {
        // TODO: How to replace existing element?
      }
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

void SpindizzyGERALDFactory::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
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

void SpindizzyGERALDFactory::loadConfiguration(DOMNodeWrapper* node, ICommandRegistry* commandRegistry) {
  cFallLimit = node->getFloatAttribute("height");
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Script") {
      cFallLimitScript = commandRegistry->getScript(mNode);
    }
  }
}
