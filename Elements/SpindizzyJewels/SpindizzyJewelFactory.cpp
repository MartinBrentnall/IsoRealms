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

SpindizzyJewelFactory::SpindizzyJewelFactory(ICollectablesAccessor* elementSet, ISimpleModelFactory* jewelModelFactory) : BaseSpindizzyJewelFactory(elementSet) {
  cJewelModelFactory = jewelModelFactory;
  BlockLocation mIdentityLocation(0, 0, 0);
  cSampleJewel = new SpindizzyJewel(this, &mIdentityLocation, cJewelModelFactory);
  cSampleJewelDynamics = cSampleJewel->getDynamicElements();
  cSampleJewelVisuals = cSampleJewel->getVisualElements();
}

std::string SpindizzyJewelFactory::getName() {
  return "";
}

void SpindizzyJewelFactory::setModel(ISimpleModelFactory* modelFactory) {
  cJewelModelFactory = modelFactory;
  cSampleJewel->setModel(cJewelModelFactory);
  for (unsigned int i = 0; i < cContent.size(); i++) {
    cContent[i]->setModel(cJewelModelFactory);
  }
}

IElement* SpindizzyJewelFactory::getElement(DOMNodeWrapper* node, BlockLocation* relative) {
  BlockLocation* mLocation = new BlockLocation();
  // TODO: Should throw something if these are not specified!
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Location") {
      mLocation->setRelative(mNode, *relative);
    }
  }
  SpindizzyJewel* mJewel = new SpindizzyJewel(this, mLocation, cJewelModelFactory);
  cContent.push_back(mJewel);
  return mJewel;
}

bool SpindizzyJewelFactory::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      SpindizzyJewel* mJewel = new SpindizzyJewel(this, cEditingLocation, cJewelModelFactory);
      cGateway->pushElement(mJewel);
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

void SpindizzyJewelFactory::setEditingInfo(BlockLocation* editingLocation, IElementGateway* gateway, IComponentContainer* componentContainer) {
  cGateway = gateway;
  cEditingLocation = editingLocation;  
}

void SpindizzyJewelFactory::renderEditingPreview() {
}

// TODO: Make this part of the IElementFactory interface!
// void SpindizzyJewelFactory::updateIcon(int milliseconds) {
//   float mColourChange = ticks * 0.0015;
//   *cColourChannel += cColourUp ? mColourChange : -mColourChange;
//   if (*cColourChannel >= 1.0 || *cColourChannel <= 0.0) {
//     cColourUp = !cColourUp;
//     *cColourChannel = *cColourChannel >= 1.0 ? 1.0 : 0.0;
//     cColourChannel = (cColourChannel == &cCurrentColour.cRed     ? &cCurrentColour.cGreen
//                    : (cColourChannel == &cCurrentColour.cGreen   ? &cCurrentColour.cBlue
//                    /* cColourChannel == *cCurrentColour.cBlue */ : &cCurrentColour.cRed));
//   }
// }

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
/*  float mRadius = 1.0f;
  float mLineWidth = 0.05;
  Vertex mBottom(0.0,      0.0,     -0.7f);
  Vertex mTop(   0.0,      0.0,      0.7f);
  Vertex mNorth( 0.0,      mRadius,  0.0f);
  Vertex mEast(  mRadius,  0.0,      0.0f);
  Vertex mSouth( 0.0,     -mRadius,  0.0f);
  Vertex mWest( -mRadius,  0.0,      0.0f);
  glColor3f(1.0f, 0.0f, 0.5f);
  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_TRIANGLES);
  renderInnerTriangle(mBottom, mNorth, mEast, mLineWidth);
  renderInnerTriangle(mBottom, mEast, mSouth, mLineWidth);
  renderInnerTriangle(mBottom, mSouth, mWest, mLineWidth);
  renderInnerTriangle(mBottom, mWest, mNorth, mLineWidth);
  renderInnerTriangle(mTop, mNorth, mWest, mLineWidth);
  renderInnerTriangle(mTop, mWest, mSouth, mLineWidth);
  renderInnerTriangle(mTop, mSouth, mEast, mLineWidth);
  renderInnerTriangle(mTop, mEast, mNorth, mLineWidth);
  glEnd();
  glColor3f(1.0, 1.0, 0.4);
  glBegin(GL_TRIANGLES);
  renderOuterTriangle(mBottom, mNorth, mEast, mLineWidth);
  renderOuterTriangle(mBottom, mEast, mSouth, mLineWidth);
  renderOuterTriangle(mBottom, mSouth, mWest, mLineWidth);
  renderOuterTriangle(mBottom, mWest, mNorth, mLineWidth);
  renderOuterTriangle(mTop, mNorth, mWest, mLineWidth);
  renderOuterTriangle(mTop, mWest, mSouth, mLineWidth);
  renderOuterTriangle(mTop, mSouth, mEast, mLineWidth);
  renderOuterTriangle(mTop, mEast, mNorth, mLineWidth);
  glEnd();*/
}

SpindizzyJewelFactory::~SpindizzyJewelFactory() {
  delete cSampleJewel;
  for (unsigned int i = 0; i < cContent.size(); i++) {
    ISimpleModel* mJewelModelInstance = cContent[i]->getModel();
    cJewelModelFactory->destroyModel(mJewelModelInstance);
    cGateway->notifyDestruction(cContent[i]);
    delete cContent[i];
  }
}

