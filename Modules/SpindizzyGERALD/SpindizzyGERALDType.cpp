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
#include "SpindizzyGERALDType.h"

SpindizzyGERALDType::SpindizzyGERALDType(ISpindizzyGERALDSet* elementSet, IRuntimeContext* runtimeContext) : ISpindizzyGERALDType(elementSet) {
  cRespawnScript = NULL;
  cFallImpactScript = NULL;
  cZoneEnteredScript = NULL;
  cZoneExitedScript = NULL;
}

SpindizzyGERALD* SpindizzyGERALDType::createInstance(const std::string& name) {
  SpindizzyGERALD* mNamedInstance = new SpindizzyGERALD(this, &cMap, cResources);
  cNamedInstances[name] = mNamedInstance;
  return mNamedInstance;
} 

void SpindizzyGERALDType::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mMapPath = node->getAttribute("map");
  std::string mModelPath = node->getAttribute("model");
  std::string mSurfaceRegistryPath = node->getAttribute("surfaceRegistry");
  std::string mCollectablesRegistryPath = node->getAttribute("collectablesRegistry");
  std::string mCameraPath = node->getAttribute("camera");
  cResources = resourceAccessor;
  for (std::map<std::string, SpindizzyGERALD*>::iterator i = cNamedInstances.begin(); i != cNamedInstances.end(); i++) {
    i->second->setResources(cResources);
  }
  cMap = resourceAccessor->getMap(mMapPath);
  cCollidableSurfaceRegistry = resourceAccessor->getSurfaceRegistry(mSurfaceRegistryPath);
  cCollectables = resourceAccessor->getCollectablesRegistry(mCollectablesRegistryPath);
  cModelType = resourceAccessor->getModelType(mModelPath);
  cCamera = resourceAccessor->getCamera(mCameraPath);
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "FallImpactScript") {
      cFallImpactScript = resourceAccessor->getLuaScript(mNode, this);
    } else if (mValueAsString == "RespawnScript") {
      cRespawnScript = resourceAccessor->getLuaScript(mNode);
    } else if (mValueAsString == "ZoneEnteredScript") {
      cZoneEnteredScript = resourceAccessor->getLuaScript(mNode, this);
    } else if (mValueAsString == "ZoneExitedScript") {
      cZoneExitedScript = resourceAccessor->getLuaScript(mNode, this);
    }
  }
  BlockLocation mIdentityLocation(0, 0, 0);
  cSampleGERALD = new SpindizzyGERALD(this, NULL, NULL, &mIdentityLocation);
}

IElement* SpindizzyGERALDType::getElement(DOMNodeWrapper* node, BlockLocation* relative, IElementContainer* container) {
  std::string mInstance = node->getAttribute("instance");
  if (mInstance == "") {
    SpindizzyGERALD* mLoadedGERALD = new SpindizzyGERALD(this, &cMap, cResources, node);
    cContent.push_back(mLoadedGERALD);
    return mLoadedGERALD;
  }
  return cNamedInstances[mInstance];
}

IElementHandler* SpindizzyGERALDType::getElementHandler() {
  ISpindizzyGERALDSet* mGERALDSet = getElementSet();
  return mGERALDSet->createHandler();
}

bool SpindizzyGERALDType::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      if (cContent.size() == 0) {
        SpindizzyGERALD* mGERALD = new SpindizzyGERALD(this, &cMap, cResources, cEditingLocation);
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

bool SpindizzyGERALDType::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void SpindizzyGERALDType::configureElement() {
  // Nothing to do.
}

void SpindizzyGERALDType::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
  cEditingLocation = editingLocation;  
}

void SpindizzyGERALDType::renderEditingPreview() {
  // Nothing to do
}

void SpindizzyGERALDType::updateIcon(unsigned int milliseconds) {
  // Nothing to do
}

void SpindizzyGERALDType::renderIcon() {
  glTranslatef(0.0f, -1.0f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glScalef(2.0f, 2.0f, 2.0f);
  cSampleGERALD->renderStatic();
  if (cSampleGERALD->isVisualRuntime()) {
    cSampleGERALD->renderRuntime();
  }
}

void SpindizzyGERALDType::saveInstances(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  BlockLocation mBlockLocation;
  for (std::map<std::string, SpindizzyGERALD*>::iterator i = cNamedInstances.begin(); i != cNamedInstances.end(); i++) {
    std::string mElementType = resourceLocator->getPath(this);
    mElementType = mElementType.substr(mElementType.find_last_of('/') + 1);
    DOMNodeWriter* mInstanceNode = node->addBranch("Instance");
    std::string mInstanceName = getInstanceName(i->second);
    mInstanceNode->addAttribute("type", mElementType);
    mInstanceNode->addAttribute("name", mInstanceName);
    i->second->saveInstance(mInstanceNode, resourceLocator, mBlockLocation);
  }
} 

void SpindizzyGERALDType::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("model", resourceLocator->getPath(cModelType));
  node->addAttribute("surfaceRegistry", resourceLocator->getPath(cCollidableSurfaceRegistry));
  node->addAttribute("collectablesRegistry", resourceLocator->getPath(cCollectables));
  node->addAttribute("camera", resourceLocator->getPath(cCamera));
  resourceLocator->saveScript(node, "RespawnScript", cRespawnScript);
  resourceLocator->saveScript(node, "FallImpactScript", cFallImpactScript);
  resourceLocator->saveScript(node, "ZoneEnteredScript", cZoneEnteredScript);
  resourceLocator->saveScript(node, "ZoneExitedScript", cZoneExitedScript);
}

void SpindizzyGERALDType::stop() {
  for (unsigned int i = 0; i < cContent.size(); i++) {
    cContent[i]->stop();
  }
}

void SpindizzyGERALDType::destroy(IElement* element) {
  delete element;
}

std::string SpindizzyGERALDType::getPath(IArgumentValue* value) {
  return value == &cArgumentZoneEntered  ? "entered"
       : value == &cArgumentZoneExited   ? "exited"
       : value == &cArgumentFallDistance ? "fallDistance"
       : value == &cArgumentCraft        ? "craft"
       :                                   "";
}

IArgumentSource* SpindizzyGERALDType::getArgument(DOMNodeWrapper* node) {
  std::string mType = node->getAttribute("type");
  std::string mInstance = node->getAttribute("value").substr(1);
  std::string mName = node->getAttribute("name");
  if (mType == "Zone") {
    if (mInstance == "entered") {
      return new ArgumentSourceLocal<IZone>(&cArgumentZoneEntered, this);
    }
    if (mInstance == "exited") {
      return new ArgumentSourceLocal<IZone>(&cArgumentZoneExited, this);
    }
  } else if (mType == "Float") {
    if (mInstance == "fallDistance") {
      return new ArgumentSourceLocal<IFloat>(&cArgumentFallDistance, this);
    }
  } else if (mType == "SpindizzyGERALD/GERALD") {
    if (mInstance == "craft") {
      return new ArgumentSourceLocal<SpindizzyGERALD>(&cArgumentCraft, this);
    }
  }
  return NULL;
}

I3DModel* SpindizzyGERALDType::createModel(Vertex* vertex) {
  return cModelType->createModel(vertex);
}

void SpindizzyGERALDType::collect(SpindizzyGERALD* gerald, Vertex& start, Vertex& end) {
  cCollectables->collect(gerald, start, end);
}

ICollisionData* SpindizzyGERALDType::getNextEvent(Vertex& start, Vertex& end, IRollableSurface* currentSurface) {
  return cCollidableSurfaceRegistry->getNextEvent(start, end, currentSurface);
}

IRollableSurface* SpindizzyGERALDType::getSurfaceAt(Vertex& location) {
  return cCollidableSurfaceRegistry->getSurfaceAt(location);
}

ICamera* SpindizzyGERALDType::getCamera() {
  return cCamera;
}

void SpindizzyGERALDType::executeRespawnScript() {
  cRespawnScript->execute();
}

void SpindizzyGERALDType::executeFallImpactScript(float fallDistance, SpindizzyGERALD* craft) {
  cArgumentCraft.setValue(craft);
  Float mFallDistance(fallDistance);
  cArgumentFallDistance.setValue(&mFallDistance);
  cFallImpactScript->execute();
}

void SpindizzyGERALDType::executeZoneEnteredScript(IZone* zone) {
  cArgumentZoneEntered.setValue(zone);
  cZoneEnteredScript->execute();
}

void SpindizzyGERALDType::executeZoneExitedScript(IZone* zone) {
  cArgumentZoneExited.setValue(zone);
  cZoneExitedScript->execute();
}

std::string SpindizzyGERALDType::getInstanceName(SpindizzyGERALD* craft) {
  for (std::map<std::string, SpindizzyGERALD*>::iterator i = cNamedInstances.begin(); i != cNamedInstances.end(); i++) {
    if (craft == i->second) {
      return i->first;
    }
  }
  return "";
}
