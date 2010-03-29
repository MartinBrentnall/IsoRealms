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

SpindizzyJewel::SpindizzyJewel(BaseSpindizzyJewelFactory* elementFactory, BlockLocation* location, ISimpleModelFactory* jewelModelFactory) : Element<ICollectablesAccessor, BaseSpindizzyJewelFactory>(elementFactory) {
  cLocation = BlockLocation(*location);
  cVertexLocation.x = cLocation.x;
  cVertexLocation.y = cLocation.y;
  cVertexLocation.z = cLocation.z;
  cModel = jewelModelFactory->createModel(&cVertexLocation);
  cCollected = false;
}

void SpindizzyJewel::setModel(ISimpleModelFactory* modelFactory) {
  cModel = modelFactory->createModel(&cVertexLocation);;
}

ISimpleModel* SpindizzyJewel::getModel() {
  return cModel;
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

std::vector<IInteractiveElement*> SpindizzyJewel::getInteractiveElements() {
  std::vector<IInteractiveElement*> mInteractiveElements;
  return mInteractiveElements;
}

void SpindizzyJewel::save(DOMNodeWriter* node, BlockLocation& location) {
  DOMNodeWriter* mLocationNode = node->addBranch("Location");
  cLocation.saveRelative(mLocationNode, location);  
}

void SpindizzyJewel::update(int milliseconds) {
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
  ICollectablesAccessor* mCollectablesAccessor = getElementSet();
  ICollectables* mCollectables = mCollectablesAccessor->getCollectables();
  mCollectables->registerCollectable(this);
  return true;
}

void SpindizzyJewel::collect() {
  cCollected = true;
  ICollectablesAccessor* mCollectablesAccessor = getElementSet();
  mCollectablesAccessor->jewelCollected();
}

bool SpindizzyJewel::isCollected(Vertex& start, Vertex& end) {
  if (!cCollected) {
    float mXMovement = end.x - start.x;
    float mYMovement = end.y - start.y;
    float mZMovement = end.z - start.z;
    float mWest   = cLocation.x - IsoRealmsConstants::BLOCK_RADIUS;
    float mEast   = cLocation.x + IsoRealmsConstants::BLOCK_RADIUS;
    float mSouth  = cLocation.y - IsoRealmsConstants::BLOCK_RADIUS;
    float mNorth  = cLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
    float mBottom = cLocation.z;
    float mTop    = cLocation.z + 1.0f;
  
    float mGradient = (mWest - start.x) / mXMovement;
    if (mGradient >= 0.0f && mGradient <= 1.0f) {
      float mYLocation = start.y + mYMovement * mGradient;
      float mZLocation = start.z + mZMovement * mGradient;
      if (mYLocation >= mSouth && mYLocation <= mNorth && mZLocation >= mBottom && mZLocation <= mTop) {
        collect();
        return true;
      }
    }
  
    mGradient = (mEast - start.x) / mXMovement;
    if (mGradient >= 0.0f && mGradient <= 1.0f) {
      float mYLocation = start.y + mYMovement * mGradient;
      float mZLocation = start.z + mZMovement * mGradient;
      if (mYLocation >= mSouth && mYLocation <= mNorth && mZLocation >= mBottom && mZLocation <= mTop) {
        collect();
        return true;
      }
    }
  
    mGradient = (mNorth - start.y) / mYMovement;
    if (mGradient >= 0.0f && mGradient <= 1.0f) {
      float mXLocation = start.x + mXMovement * mGradient;
      float mZLocation = start.z + mZMovement * mGradient;
      if (mXLocation >= mWest && mXLocation <= mEast && mZLocation >= mBottom && mZLocation <= mTop) {
        collect();
        return true;
      }
    }
  
    mGradient = (mSouth - start.y) / mYMovement;
    if (mGradient >= 0.0f && mGradient <= 1.0f) {
      float mXLocation = start.x + mXMovement * mGradient;
      float mZLocation = start.z + mZMovement * mGradient;
      if (mXLocation >= mWest && mXLocation <= mEast && mZLocation >= mBottom && mZLocation <= mTop) {
        collect();
        return true;
      }
    }
  
    mGradient = (mTop - start.z) / mYMovement;
    if (mGradient >= 0.0f && mGradient <= 1.0f) {
      float mXLocation = start.x + mXMovement * mGradient;
      float mYLocation = start.y + mYMovement * mGradient;
      if (mXLocation >= mWest && mXLocation <= mEast && mYLocation >= mSouth && mYLocation <= mNorth) {
        collect();
        return true;
      }
    }
  
    mGradient = (mBottom - start.z) / mYMovement;
    if (mGradient >= 0.0f && mGradient <= 1.0f) {
      float mXLocation = start.x + mXMovement * mGradient;
      float mYLocation = start.y + mYMovement * mGradient;
      if (mXLocation >= mWest && mXLocation <= mEast && mYLocation >= mSouth && mYLocation <= mNorth) {
        collect();
        return true;
      }
    }
  }
  return false;
}

void SpindizzyJewel::setDirty() {
  signalElementDirty();
}
