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
#include "SpindizzyJewelFactory.h"

SpindizzyJewelFactory::SpindizzyJewelFactory(ISpindizzyJewelSet* elementSet, DOMNodeWrapper* node, IRuntimeContext* runtimeContext) : BaseSpindizzyJewelFactory(elementSet) {
  cRuntimeContext = runtimeContext;
  cName = node->getAttribute("name");
  cModelPath = node->getAttribute("model");
  BlockLocation mIdentityLocation(0, 0, 0);
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "JewelCollectedScript") {
      cJewelCollectedScript = cRuntimeContext->getScript(mNode);
    } else if (mValueAsString == "AllJewelsCollectedScript") {
      cAllJewelsCollectedScript = cRuntimeContext->getScript(mNode);
    } else {
      // TODO: Throw something!
    }
  }
  cSampleJewel = new SpindizzyJewel(this, &mIdentityLocation, cModelPath, cRuntimeContext);
  cSampleJewelDynamics = cSampleJewel->getDynamicElements();
  cSampleJewelVisuals = cSampleJewel->getVisualElements();
}

std::string SpindizzyJewelFactory::getName() {
  return cName;
}

IElement* SpindizzyJewelFactory::getElement(DOMNodeWrapper* node, BlockLocation* relative, IElementContainer* elementContainer) {
  BlockLocation* mLocation = new BlockLocation();
  // TODO: Should throw something if these are not specified!
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Location") {
      mLocation->setRelative(mNode, *relative);
    }
  }
  SpindizzyJewel* mJewel = new SpindizzyJewel(this, mLocation, cModelPath, cRuntimeContext);
  cContent.push_back(mJewel);
  return mJewel;
}

bool SpindizzyJewelFactory::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      SpindizzyJewel* mJewel = new SpindizzyJewel(this, cEditingLocation, cModelPath, cRuntimeContext);
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

bool SpindizzyJewelFactory::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void SpindizzyJewelFactory::configureElement() {
  // Nothing to do.
}

void SpindizzyJewelFactory::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
  cEditingLocation = editingLocation;  
}

void SpindizzyJewelFactory::renderEditingPreview() {
}

void SpindizzyJewelFactory::updateIcon(int milliseconds) {
  for (unsigned int i = 0; i < cSampleJewelDynamics.size(); i++) {
    cSampleJewelDynamics[i]->update(milliseconds);
  }
}

void SpindizzyJewelFactory::setDirty(IElement* element) {
  // Nothing to do for this element
}

void SpindizzyJewelFactory::renderIcon() {
  glTranslatef(0.0f, -0.6f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glScalef(2.0f, 2.0f, 2.0f);
  cSampleJewel->renderStatic();
  for (unsigned int i = 0; i < cSampleJewelVisuals.size(); i++) {
    cSampleJewelVisuals[i]->render();
  }
}

void SpindizzyJewelFactory::jewelCollected() {
  cJewelCollectedScript->execute();
}

void SpindizzyJewelFactory::allJewelsCollected() {
  cAllJewelsCollectedScript->execute();
}

/* TODO: Save this type
void SpindizzyJewelFactory::save(DOMNodeWriter* node) {
  cJewelCollectedScript->save(node, "JewelCollectedScript");
  cAllJewelsCollectedScript->save(node, "AllJewelsCollectedScript");
}
*/

SpindizzyJewelFactory::~SpindizzyJewelFactory() {
  delete cSampleJewel;
  for (unsigned int i = 0; i < cContent.size(); i++) {
//    ISimpleModel* mJewelModelInstance = cContent[i]->getModel();
// TODO: Destroy the model    cJewelModelFactory->destroyModel(mJewelModelInstance);
    removeElement(cContent[i]);
    delete cContent[i];
  }
}

