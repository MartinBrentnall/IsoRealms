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
#include "SpindizzyJewelType.h"

SpindizzyJewelType::SpindizzyJewelType(ISpindizzyJewelSet* elementSet) : BaseSpindizzyJewelType(elementSet) {
  cJewelCollectedScript = NULL;
  cAllJewelsCollectedScript = NULL;
}

void SpindizzyJewelType::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mModelPath = node->getAttribute("model");
  cModelType = resourceAccessor->getModelType(mModelPath);
  std::string mCollectablesPath = node->getAttribute("collectablesRegistry");
  cCollectables = resourceAccessor->getCollectablesRegistry(mCollectablesPath);
  BlockLocation mIdentityLocation(0, 0, 0);
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "JewelCollectedScript") {
      cJewelCollectedScript = resourceAccessor->getLuaScript(mNode);
    } else if (mValueAsString == "AllJewelsCollectedScript") {
      cAllJewelsCollectedScript = resourceAccessor->getLuaScript(mNode);
    } else {
      // TODO: Throw something!
    }
  }
  cSampleJewel = new SpindizzyJewel(this, &mIdentityLocation, cModelType);
  cSampleJewelDynamics = cSampleJewel->getDynamicElements();
  cSampleJewelVisuals = cSampleJewel->getVisualElements();
}

void SpindizzyJewelType::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("model", resourceLocator->getPath(cModelType));
  node->addAttribute("collectablesRegistry", resourceLocator->getPath(cCollectables));
  if (cJewelCollectedScript != NULL) {
    DOMNodeWriter* mJewelCollectedScriptNode = node->addBranch("JewelCollectedScript");
    cJewelCollectedScript->save(mJewelCollectedScriptNode, resourceLocator);
  }
  if (cAllJewelsCollectedScript != NULL) {
    DOMNodeWriter* mAllCollectedScriptNode = node->addBranch("AllJewelsCollectedScript");
    cAllJewelsCollectedScript->save(mAllCollectedScriptNode, resourceLocator);
  }
}

IElement* SpindizzyJewelType::getElement(DOMNodeWrapper* node, BlockLocation* relative, IElementContainer* container) {
  BlockLocation* mLocation = new BlockLocation();
  mLocation->setRelative(node, *relative);
  SpindizzyJewel* mJewel = new SpindizzyJewel(this, mLocation, cModelType);
  cContent.push_back(mJewel);
  return mJewel;
}

IElementHandler* SpindizzyJewelType::getElementHandler() {
  ISpindizzyJewelSet* mSpindizzyJewelSet = getElementSet();
  return mSpindizzyJewelSet->createHandler();
}

bool SpindizzyJewelType::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      SpindizzyJewel* mJewel = new SpindizzyJewel(this, cEditingLocation, cModelType);
      addElement(mJewel);
      cContent.push_back(mJewel);
      return true;
    }

    default: {
      return false;
    }
  }
  return false;
}

bool SpindizzyJewelType::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void SpindizzyJewelType::configureElement() {
  // Nothing to do.
}

void SpindizzyJewelType::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
  cEditingLocation = editingLocation;  
}

void SpindizzyJewelType::renderEditingPreview() {
}

void SpindizzyJewelType::updateIcon(int milliseconds) {
  for (unsigned int i = 0; i < cSampleJewelDynamics.size(); i++) {
    cSampleJewelDynamics[i]->update(milliseconds);
  }
}

void SpindizzyJewelType::setDirty(IElement* element) {
  // Nothing to do for this element
}

void SpindizzyJewelType::renderIcon() {
  glTranslatef(0.0f, -0.6f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glScalef(2.0f, 2.0f, 2.0f);
  cSampleJewel->renderStatic();
  for (unsigned int i = 0; i < cSampleJewelVisuals.size(); i++) {
    cSampleJewelVisuals[i]->render();
  }
}

ICollectables* SpindizzyJewelType::getCollectables() {
  return cCollectables;
}

void SpindizzyJewelType::jewelCollected() {
  ISpindizzyJewelSet* mJewelSet = getElementSet();
  mJewelSet->itemCollected();
  if (cJewelCollectedScript != NULL) {
    cJewelCollectedScript->execute();
  }
}

void SpindizzyJewelType::allJewelsCollected() {
  if (cAllJewelsCollectedScript != NULL) {
    cAllJewelsCollectedScript->execute();
  }
}

/* TODO: Save this type
void SpindizzyJewelType::save(DOMNodeWriter* node) {
  cJewelCollectedScript->save(node, "JewelCollectedScript");
  cAllJewelsCollectedScript->save(node, "AllJewelsCollectedScript");
}
*/

void SpindizzyJewelType::destroy(IElement* jewel) {
  delete jewel;
}

SpindizzyJewelType::~SpindizzyJewelType() {
  delete cSampleJewel;
  for (unsigned int i = 0; i < cContent.size(); i++) {
//    ISimpleModel* mJewelModelInstance = cContent[i]->getModel();
// TODO: Destroy the model    cJewelModelType->destroyModel(mJewelModelInstance);
    removeElement(cContent[i]);
    delete cContent[i];
  }
}

