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
#include "ElementSpindizzyItem.h"

ElementSpindizzyItem::ElementSpindizzyItem(ISpindizzyJewelType* elementType, BlockLocation* location, I3DModelType* modelType, IElementContainer* container) {
  cItemType = elementType;
  cLocation = BlockLocation(*location);
  cVertexLocation.x = cLocation.x;
  cVertexLocation.y = cLocation.y;
  cVertexLocation.z = cLocation.z;
  cModel = modelType->createModel(&cVertexLocation);
  cCollected = false;
  cContainer = container;
}

void ElementSpindizzyItem::setModelType(I3DModelType* oldModelType, I3DModelType* newModelType) {
  oldModelType->destroyModel(cModel);
  cModel = newModelType->createModel(&cVertexLocation);
}

bool ElementSpindizzyItem::isCollected() {
  return cCollected;
}

IElementContainer* ElementSpindizzyItem::getElementContainer() {
  return cContainer;
}

void ElementSpindizzyItem::renderStatic() {
  // Nothing to do.
}

void ElementSpindizzyItem::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& location) {
  cLocation.saveRelative(node, location);  
}

IElementBounds* ElementSpindizzyItem::getBounds() {
  return nullptr; // TODO: Implement this
}

void ElementSpindizzyItem::updateRuntime(unsigned int milliseconds) {
  if (!cCollected) {
    cModel->update(milliseconds);
  }
}

void ElementSpindizzyItem::renderRuntime() {
  if (!cCollected) {
    glPushMatrix();
    cModel->render();
    glPopMatrix();
  }
}

IElementType* ElementSpindizzyItem::getElementType() {
  return cItemType;
}

bool ElementSpindizzyItem::initElement(unsigned int) {
  ISpindizzyJewelSet* mSpindizzyJewelSet = cItemType->getSpindizzyItemInterface();
  if (!mSpindizzyJewelSet->isEditing()) {
    IBoundaries* mCollectables = cItemType->getCollectables();
    mCollectables->registerBoundary(this);
  }
  return true;
}

void ElementSpindizzyItem::collect() {
  cCollected = true;
}

bool ElementSpindizzyItem::contains(Vertex& location) {
  if (!cCollected) {
    float mWest   = cLocation.x - IsoRealmsConstants::BLOCK_RADIUS;
    float mEast   = cLocation.x + IsoRealmsConstants::BLOCK_RADIUS;
    float mSouth  = cLocation.y - IsoRealmsConstants::BLOCK_RADIUS;
    float mNorth  = cLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
    float mBottom = cLocation.z - 0.25f;
    float mTop    = cLocation.z + 1.0f;
    if (Collision::contains(location, mWest, mEast, mSouth, mNorth, mBottom, mTop)) {
      collect();
      return true;
    }
  }
  return false;
}

bool ElementSpindizzyItem::isEntered(Vertex& start, Vertex& end) {
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

bool ElementSpindizzyItem::isExited(Vertex& start, Vertex& end) {
  return false; // Not supported for items
}

void ElementSpindizzyItem::setDirty() {
  // TODO: signalElementDirty();
}

BlockArea* ElementSpindizzyItem::getCoverage() {
  return new BlockArea(cLocation, cLocation);
}

void ElementSpindizzyItem::setArguments() {
  cContainer->setArguments();
}

void ElementSpindizzyItem::unsetArguments() {
  cContainer->unsetArguments();
}
