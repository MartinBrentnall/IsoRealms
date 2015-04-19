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
  cJewelCollectedScript = nullptr;
  cAllJewelsCollectedScript = nullptr;
}

void ResourceElementSpindizzyItem::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mModelPath = node->getAttribute("model");
  cModelType = resourceAccessor->getModelType(mModelPath);
  std::string mBoundariesPath = node->getAttribute("boundaries");
  cBoundaries = resourceAccessor->getBoundaries(mBoundariesPath);
  BlockLocation mIdentityLocation(0, 0, 0);
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "JewelCollectedScript") {
      cJewelCollectedScript = resourceAccessor->getScriptCall(mNode);
    } else if (mValueAsString == "AllJewelsCollectedScript") {
      cAllJewelsCollectedScript = resourceAccessor->getScriptCall(mNode);
    } else {
      // TODO: Throw something!
    }
  }
  cSampleJewel = new ElementSpindizzyItem(this, &mIdentityLocation, cModelType, nullptr);
}

void ResourceElementSpindizzyItem::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("model", resourceLocator->getPath(cModelType));
  node->addAttribute("boundaries", resourceLocator->getPath(cBoundaries));
  if (cJewelCollectedScript != nullptr) {
    DOMNodeWriter* mJewelCollectedScriptNode = node->addBranch("JewelCollectedScript");
    cJewelCollectedScript->save(mJewelCollectedScriptNode, resourceLocator);
  }
  if (cAllJewelsCollectedScript != nullptr) {
    DOMNodeWriter* mAllCollectedScriptNode = node->addBranch("AllJewelsCollectedScript");
    cAllJewelsCollectedScript->save(mAllCollectedScriptNode, resourceLocator);
  }
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

bool ResourceElementSpindizzyItem::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      ElementSpindizzyItem* mJewel = new ElementSpindizzyItem(this, cEditingLocation, cModelType, nullptr);
//      addElement(mJewel);
      cContent.push_back(mJewel);
      return true;
    }

    default: {
      return false;
    }
  }
  return false;
}

bool ResourceElementSpindizzyItem::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void ResourceElementSpindizzyItem::configureElement() {
  // Nothing to do.
}

void ResourceElementSpindizzyItem::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
  cEditingLocation = editingLocation;  
}

void ResourceElementSpindizzyItem::renderEditingPreview() {
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

void ResourceElementSpindizzyItem::jewelCollected() {
  cModuleInterface->itemCollected();
  if (cJewelCollectedScript != nullptr) {
    cJewelCollectedScript->execute();
  }
}

void ResourceElementSpindizzyItem::allJewelsCollected() {
  if (cAllJewelsCollectedScript != nullptr) {
    cAllJewelsCollectedScript->execute();
  }
}

/* TODO: Save this type
void ResourceElementSpindizzyItem::save(DOMNodeWriter* node) {
  cJewelCollectedScript->save(node, "JewelCollectedScript");
  cAllJewelsCollectedScript->save(node, "AllJewelsCollectedScript");
}
*/

void ResourceElementSpindizzyItem::destroy(IElement* jewel) {
  delete jewel;
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

