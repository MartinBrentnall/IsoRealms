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

SpindizzyGERALDFactory::SpindizzyGERALDFactory(ISpindizzyGERALDSet* elementSet, ILocationAwareness* locationAwareness, IZoneContext* zoneContext, ICollidableSurfaceRegistry* collidableSurfaceRegistry, ICollectables* collectables, ICamera* camera, DOMNodeWrapper* node, IRuntimeContext* runtimeContext) : ISpindizzyGERALDFactory(elementSet) {
  cRuntimeContext = runtimeContext;
  cProject = cRuntimeContext->getProject();
  cFallScript = NULL;
  cFallLimitScript = NULL;
  cLocationAwareness = locationAwareness;
  cZoneContext = zoneContext;
  cCollidableSurfaceRegistry = collidableSurfaceRegistry;
  cCollectables = collectables;
  cCamera = camera;
  cName = node->getAttribute("type");
  cModelPath = node->getAttribute("model");
  BlockLocation mIdentityLocation(0, 0, 0);
  cSampleGERALD = new SpindizzyGERALD(this, &mIdentityLocation, cModelPath, NULL, NULL, NULL, NULL, NULL, 0.0f, NULL, NULL, NULL, cRuntimeContext);
  cSampleGERALDVisuals = cSampleGERALD->getVisualElements();

  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "FallLimitScript") {
      loadFallLimitConfiguration(mNode, runtimeContext);
    } else if (mValueAsString == "RespawnScript") {
      loadRespawnConfiguration(mNode, runtimeContext);
    }
  }
}

std::string SpindizzyGERALDFactory::getName() {
  return cName;
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
  SpindizzyGERALD* mLoadedGERALD = new SpindizzyGERALD(this, &mStartLocation, cModelPath, cCollectables, cCollidableSurfaceRegistry, cLocationAwareness, cZoneContext, cCamera, cFallLimit, cFallLimitScript, cFallScript, cProject, cRuntimeContext);
  cContent.push_back(mLoadedGERALD);
  return mLoadedGERALD;
}

bool SpindizzyGERALDFactory::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      if (cContent.size() == 0) {
        SpindizzyGERALD* mGERALD = new SpindizzyGERALD(this, cEditingLocation, cModelPath, cCollectables, cCollidableSurfaceRegistry, cLocationAwareness, cZoneContext, cCamera, cFallLimit, cFallLimitScript, cFallScript, cProject, cRuntimeContext);
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

void SpindizzyGERALDFactory::save(DOMNodeWriter* node) {
  DOMNodeWriter* mFallLimitNode = node->addBranch("FallLimitScript");
  mFallLimitNode->addAttribute("height", cFallLimit);
//  cFallLimitScript->save(mFallLimitNode, "Script");
//  DOMNodeWriter* mRespawnNode = node->addBranch("Respawn");
//  cFallScript->save(mRespawnNode, "Script");
}

void SpindizzyGERALDFactory::loadFallLimitConfiguration(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  cFallLimit = node->getFloatAttribute("height");
  cFallLimitScript = runtimeContext->getLuaScript(node);
}

void SpindizzyGERALDFactory::loadRespawnConfiguration(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  cFallScript = runtimeContext->getLuaScript(node);
}

void SpindizzyGERALDFactory::stop() {
  for (unsigned int i = 0; i < cContent.size(); i++) {
    cContent[i]->stop();
  }
}
