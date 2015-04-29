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
#include "ResourceElementSpindizzyCraft.h"

ResourceElementSpindizzyCraft::ResourceElementSpindizzyCraft(ISpindizzyGERALDSet* elementSet, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cModuleInterface = elementSet;
  cRespawnScript = nullptr;
  cFallImpactScript = nullptr;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Instance") {
      std::string mName = mNode->getAttribute("name");
      ElementSpindizzyCraft* mCraftInstance = createInstance(mName);
      IArgumentValue* mArgumentValue = new ArgumentValue<ElementSpindizzyCraft>(mCraftInstance);
      Vertex* mCraftLocation = mCraftInstance->getLocation();
      resourceRegistry->add(mCraftInstance, mNode);
      resourceRegistry->add(mCraftLocation, mName);
      resourceRegistry->add(mArgumentValue, "GERALD", mName);
      cContent.push_back(mCraftInstance);
    }
  }
}

ElementSpindizzyCraft* ResourceElementSpindizzyCraft::createInstance(const std::string& name) {
  ElementSpindizzyCraft* mNamedInstance = new ElementSpindizzyCraft(this);
  cNamedInstances[name] = mNamedInstance;
  return mNamedInstance;
} 

void ResourceElementSpindizzyCraft::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mMapPath = node->getAttribute("map");
  std::string mModelPath = node->getAttribute("model");
  std::string mSurfaceRegistryPath = node->getAttribute("surfaceRegistry");
  std::string mCameraPath = node->getAttribute("camera");
  cResources = resourceAccessor;
//   for (std::map<std::string, ElementSpindizzyCraft*>::iterator i = cNamedInstances.begin(); i != cNamedInstances.end(); i++) {
//     i->second->setResources(cResources);
//   }
  cModelType = resourceAccessor->getModelType(mModelPath);
  cCamera = resourceAccessor->getCamera(mCameraPath);
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "FallImpactScript") {
      cFallImpactScript = resourceAccessor->getScriptCall(mNode, this);
    } else if (mValueAsString == "RespawnScript") {
      cRespawnScript = resourceAccessor->getScriptCall(mNode);
    } else if (mValueAsString == "Boundaries") {
      IBoundaryHandler* mBoundaryHandler = resourceAccessor->getBoundaryHandler(mNode);
      cBoundaryHandlers.push_back(mBoundaryHandler);
    }
  }
  BlockLocation mIdentityLocation(0, 0, 0);
  cSampleGERALD = new ElementSpindizzyCraft(this, nullptr, &mIdentityLocation);
  
  for (std::map<std::string, ElementSpindizzyCraft*>::iterator i = cNamedInstances.begin(); i != cNamedInstances.end(); i++) {
    i->second->setModel(cModelType);
  }
}

void ResourceElementSpindizzyCraft::loadElement(DOMNodeWrapper* node, BlockLocation* relative, IElementContainer* container, IResourceAccessor* resources) {
  std::string mInstance = node->getAttribute("instance");
  if (mInstance == "") {
    ElementSpindizzyCraft* mLoadedGERALD = new ElementSpindizzyCraft(this, cResources, node);
    cContent.push_back(mLoadedGERALD);
  } else {
    ElementSpindizzyCraft* mCraftInstance = cNamedInstances[mInstance];
    mCraftInstance->setElementContainer(container);
    container->addElement(mCraftInstance);
  }
}

bool ResourceElementSpindizzyCraft::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      if (cContent.size() == 0) {
        ElementSpindizzyCraft* mGERALD = new ElementSpindizzyCraft(this, cResources, cEditingLocation);
//        addElement(mGERALD);
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

bool ResourceElementSpindizzyCraft::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void ResourceElementSpindizzyCraft::configureElement() {
  // Nothing to do.
}

void ResourceElementSpindizzyCraft::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
  cEditingLocation = editingLocation;  
}

void ResourceElementSpindizzyCraft::renderEditingPreview() {
  // Nothing to do
}

void ResourceElementSpindizzyCraft::updateIcon(unsigned int milliseconds) {
  // Nothing to do
}

void ResourceElementSpindizzyCraft::renderIcon() {
  glTranslatef(0.0f, -1.0f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glScalef(2.0f, 2.0f, 2.0f);
  cSampleGERALD->renderStatic();
  if (cSampleGERALD->isVisualRuntime()) {
    cSampleGERALD->renderRuntime();
  }
}

void ResourceElementSpindizzyCraft::saveInstances(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  BlockLocation mBlockLocation;
  for (std::map<std::string, ElementSpindizzyCraft*>::iterator i = cNamedInstances.begin(); i != cNamedInstances.end(); i++) {
    std::string mElementType = resourceLocator->getPath(this);
    mElementType = mElementType.substr(mElementType.find_last_of('/') + 1);
    DOMNodeWriter* mInstanceNode = node->addBranch("Instance");
    std::string mInstanceName = getInstanceName(i->second);
    mInstanceNode->addAttribute("type", mElementType);
    mInstanceNode->addAttribute("name", mInstanceName);
    i->second->saveInstance(mInstanceNode, resourceLocator, mBlockLocation);
  }
} 

void ResourceElementSpindizzyCraft::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("model", resourceLocator->getPath(cModelType));
  node->addAttribute("camera", resourceLocator->getPath(cCamera));
  resourceLocator->saveScript(node, "RespawnScript", cRespawnScript);
  resourceLocator->saveScript(node, "FallImpactScript", cFallImpactScript);
  for (unsigned int i = 0; i < cBoundaryHandlers.size(); i++) {
//    DOMNodeWriter* mBoundariesNode = node->addBranch("Boundaries");
//    mBoundariesNode->addAttribute("name", resourceLocator->getPath(cBoundaries[i]));
  }
}

void ResourceElementSpindizzyCraft::stop() {
  for (unsigned int i = 0; i < cContent.size(); i++) {
    cContent[i]->stop();
  }
}

void ResourceElementSpindizzyCraft::destroy(IElement* element) {
  delete element;
}

std::string ResourceElementSpindizzyCraft::getPath(IArgumentValue* value) {
  return
//          value == &cArgumentZoneEntered  ? "entered"
//        : value == &cArgumentZoneExited   ? "exited"
         value == &cArgumentFallDistance ? "fallDistance"
       : value == &cArgumentCraft        ? "craft"
       :                                   "";
}

IArgumentValue* ResourceElementSpindizzyCraft::getArgumentValue(DOMNodeWrapper* node) {
  std::string mType = node->getAttribute("type");
  std::string mInstance = node->getAttribute("value").substr(1);
  std::string mName = node->getAttribute("name");
//   if (mType == "Zone") {
//     if (mInstance == "entered") {
//       return new ArgumentSourceLocal<IZone>(&cArgumentZoneEntered, this);
//     }
//     if (mInstance == "exited") {
//       return new ArgumentSourceLocal<IZone>(&cArgumentZoneExited, this);
//     }
//   } else 
            if (mType == "Float") {
    if (mInstance == "fallDistance") {
      return &cArgumentFallDistance;
    }
  } else if (mType == "Spindizzy/GERALD") {
    if (mInstance == "craft") {
      return &cArgumentCraft;
    }
  }
  return nullptr;
}

ISpindizzyGERALDSet* ResourceElementSpindizzyCraft::getSpindizzyCraftInterface() {
  return cModuleInterface;
}

I3DModel* ResourceElementSpindizzyCraft::createModel(Vertex* vertex) {
  return cModelType->createModel(vertex);
}

void ResourceElementSpindizzyCraft::notifyMovement(ElementSpindizzyCraft* gerald, Vertex& start, Vertex& end) {
  for (unsigned int i = 0; i < cBoundaryHandlers.size(); i++) {
    cBoundaryHandlers[i]->notifyMovement(gerald, start, end);
  }
}

void ResourceElementSpindizzyCraft::notifyAppearance(ElementSpindizzyCraft* craft, Vertex& location) {
  for (unsigned int i = 0; i < cBoundaryHandlers.size(); i++) {
    cBoundaryHandlers[i]->notifyAppearance(craft, location);
  }
}

void ResourceElementSpindizzyCraft::notifyDisappearance(ElementSpindizzyCraft* craft, Vertex& location) {
  for (unsigned int i = 0; i < cBoundaryHandlers.size(); i++) {
    cBoundaryHandlers[i]->notifyDisappearance(craft, location);
  }
}

ICollisionData* ResourceElementSpindizzyCraft::getNextEvent(Vertex& start, Vertex& end, IRollableSurface* currentSurface) {
  return cModuleInterface->getNextEvent(start, end, currentSurface);
}

IRollableSurface* ResourceElementSpindizzyCraft::getSurfaceAt(Vertex& location) {
  return cModuleInterface->getSurfaceAt(location);
}

ICamera* ResourceElementSpindizzyCraft::getCamera() {
  return cCamera;
}

void ResourceElementSpindizzyCraft::executeRespawnScript() {
  cRespawnScript->execute();
}

void ResourceElementSpindizzyCraft::executeFallImpactScript(float fallDistance, ElementSpindizzyCraft* craft) {
  cArgumentCraft.setValue(craft);
  Float mFallDistance(fallDistance);
  cArgumentFallDistance.setValue(&mFallDistance);
  cFallImpactScript->execute();
}

std::string ResourceElementSpindizzyCraft::getInstanceName(ElementSpindizzyCraft* craft) {
  for (std::map<std::string, ElementSpindizzyCraft*>::iterator i = cNamedInstances.begin(); i != cNamedInstances.end(); i++) {
    if (craft == i->second) {
      return i->first;
    }
  }
  return "";
}

ResourceElementSpindizzyCraft::~ResourceElementSpindizzyCraft() {
  delete cSampleGERALD;
  for (unsigned int i = 0; i < cContent.size(); i++) {
    IElementContainer* mContainer = cContent[i]->getElementContainer();
    mContainer->removeElement(cContent[i]);
    delete cContent[i];
  }  
}
