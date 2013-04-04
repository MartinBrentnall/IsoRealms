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
#include "SpindizzyJewel.h"

SpindizzyJewel::SpindizzyJewel(BaseSpindizzyJewelType* elementType, BlockLocation* location, I3DModelFactory* modelType) : Element<ISpindizzyJewelSet, BaseSpindizzyJewelType>(elementType) {
  cLocation = BlockLocation(*location);
  cVertexLocation.x = cLocation.x;
  cVertexLocation.y = cLocation.y;
  cVertexLocation.z = cLocation.z;
  cModel = modelType->createModel(&cVertexLocation);
  cCollected = false;
}

void SpindizzyJewel::renderStatic() {
  // Nothing to do.
}

std::vector<IVisualElement*> SpindizzyJewel::getVisualElements() {
  std::vector<IVisualElement*> mVisualElements;
  mVisualElements.push_back(this);
  return mVisualElements;
}

std::vector<IDynamicElement*> SpindizzyJewel::getDynamicElements() {
  std::vector<IDynamicElement*> mDynamicElements;
  mDynamicElements.push_back(this);
  return mDynamicElements;
}

std::vector<IDynamicElement*> SpindizzyJewel::getDynamicElementsRuntime() {
  std::vector<IDynamicElement*> mDynamicElements;
  mDynamicElements.push_back(this);
  return mDynamicElements;
}

std::vector<IInteractiveElement*> SpindizzyJewel::getInteractiveElements() {
  std::vector<IInteractiveElement*> mInteractiveElements;
  return mInteractiveElements;
}

void SpindizzyJewel::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& location) {
  cLocation.saveRelative(node, location);  
}

void SpindizzyJewel::update(unsigned int milliseconds) {
  if (!cCollected) {
    cModel->update(milliseconds);
  }
}

void SpindizzyJewel::render() {
  if (!cCollected) {
    glPushMatrix();
    cModel->render();
    glPopMatrix();
  }
}

bool SpindizzyJewel::initElement(unsigned int) {
  ISpindizzyJewelSet* mSpindizzyJewelSet = getElementSet();
  if (!mSpindizzyJewelSet->isEditing()) {
    BaseSpindizzyJewelType* mSpindizzyJewelType = getElementType();
    ICollectables* mCollectables = mSpindizzyJewelType->getCollectables();
    mCollectables->registerCollectable(this);
  }
  return true;
}

void SpindizzyJewel::collect() {
  cCollected = true;
  BaseSpindizzyJewelType* mSpindizzyJewelType = getElementType();
  mSpindizzyJewelType->jewelCollected();
}

bool SpindizzyJewel::isCollected(Vertex& start, Vertex& end) {
  if (!cCollected) {
    float mWest   = cLocation.x - IsoRealmsConstants::BLOCK_RADIUS;
    float mEast   = cLocation.x + IsoRealmsConstants::BLOCK_RADIUS;
    float mSouth  = cLocation.y - IsoRealmsConstants::BLOCK_RADIUS;
    float mNorth  = cLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
    float mBottom = cLocation.z - 0.25f;
    float mTop    = cLocation.z + 1.0f;
    if (Collision::hasCrossed(start, end, mWest, mEast, mSouth, mNorth, mBottom, mTop, true)) {
      collect();
      return true;
    }
  }
  return false;
}

void SpindizzyJewel::setDirty() {
  // TODO: signalElementDirty();
}
