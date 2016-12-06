/*
 * Copyright 2015 Martin Brentnall
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
#ifndef SPATIAL_CONTAINER_2D_H
#define SPATIAL_CONTAINER_2D_H

#include <iostream>
#include <vector>

#include <IsoRealms/Resources/ElementType/IElementBounds.h>

#include "SpatialContainer1D.h"

template <class T> class SpatialContainer2D {
  private:
  SpatialContainer1D<SpatialContainer1D<std::vector<T*> > > cContainer;

  public:
  SpatialContainer2D<T>() {
    cContainer.setUnit(8);
  }
    
  void add(T* element) {
    IElementBounds* mBounds = element->getBounds();
    int mSouth = std::floor(mBounds->getSouth());
    int mNorth = std::ceil( mBounds->getNorth());
    int mWest  = std::floor(mBounds->getWest());
    int mEast  = std::ceil( mBounds->getEast());
    SpatialContainer1D<std::vector<T*> >* mRow = cContainer.getInsertionCell(mSouth, mNorth);
    std::vector<T*>* mCell = mRow->getInsertionCell(mWest, mEast);
    mCell->push_back(element);
  }

  void remove(T* element) {
    std::vector<SpatialContainer1D<std::vector<T*> >*> mRows = cContainer.getAllCells();
    for (unsigned int i = 0; i < mRows.size(); i++) {
      if (mRows[i] != nullptr) {
        std::vector<std::vector<T*>*> mCells = mRows[i]->getAllCells();
        for (unsigned int j = 0; j < mCells.size(); j++) {
          std::vector<T*>* mCell = mCells[j];
          if (mCell != nullptr) {
            for (int k = mCell->size() - 1; k >= 0; k--) {
              if ((*mCell)[k] == element) {
                mCell->erase(mCell->begin() + k);
              }
            }
          }
        }
      }
    }
  }
  
  bool contains(T* element) {
    IElementBounds* mBounds = element->getBounds();
    float mWest  = mBounds->getWest();
    float mEast  = mBounds->getEast();
    float mSouth = mBounds->getSouth();
    float mNorth = mBounds->getNorth();
    std::vector<T*> mNearbyElements = getElements(mSouth, mNorth, mWest, mEast);
    for (T* mElement : mNearbyElements) {
      if (mElement == element) {
        return true;
      }
    }
    return false;
  }
  
  void clear() {
    std::vector<SpatialContainer1D<std::vector<T*> > > mRows = cContainer.getAllElements();
    for (unsigned int i = 0; i < mRows.size(); i++) {
      std::vector<std::vector<T*> > mCells = mRows[i].getAllElements();
      for (unsigned int j = 0; j < mCells.size(); j++) {
        mCells[j].clear();
      }
      mRows[i].clear();
    }
    cContainer.clear();
  }
 
  std::vector<T*> getElements() {
    std::vector<T*> mResult;
    std::vector<SpatialContainer1D<std::vector<T*> >*> mRows = cContainer.getElements();
    for (unsigned int i = 0; i < mRows.size(); i++) {
      if (mRows[i] != nullptr) {
        std::vector<std::vector<T*>*> mCells = mRows[i]->getElements();
        for (unsigned int j = 0; j < mCells.size(); j++) {
          if (mCells[j] != nullptr) {
            mResult.insert(mResult.begin(), mCells[j]->begin(), mCells[j]->end());
          }
        }
      }
    }
    return mResult;
  }
 
  std::vector<T*> getElements(int x, int y) {
    return getElements(y, y, x, x);
  }
 
  std::vector<T*> getElements(Vertex& start, Vertex& end) {
    int mSouth = std::min(start.y, end.y);
    int mNorth = std::max(start.y, end.y);
    int mWest  = std::min(start.x, end.x);
    int mEast  = std::max(start.x, end.x);
    return getElements(mSouth, mNorth, mWest, mEast);
  }
 
  std::vector<T*> getElements(int south, int north, int west, int east) {
    std::vector<T*> mResult;
    std::vector<SpatialContainer1D<std::vector<T*> >*> mRows = cContainer.getElements(south, north);
    for (unsigned int i = 0; i < mRows.size(); i++) {
      if (mRows[i] != nullptr) {
        std::vector<std::vector<T*>*> mCells = mRows[i]->getElements(west, east);
        for (unsigned int j = 0; j < mCells.size(); j++) {
          if (mCells[j] != nullptr) {
            mResult.insert(mResult.begin(), mCells[j]->begin(), mCells[j]->end());
          }
        }
      }
    }
    return mResult;
  }
};

#endif
