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
#ifndef SPATIAL_CONTAINER_1D_H
#define SPATIAL_CONTAINER_1D_H

#include <cmath>
#include <vector>

template <class T> class SpatialContainer1D {
  private:
  unsigned int cUnit;
  std::vector<T*> cCells;
  int cStepOffset;
  SpatialContainer1D<T>* cBiggerSpace;
 
  public:
  SpatialContainer1D<T>() {
    cUnit = 8;
    cStepOffset = 0;
    cBiggerSpace = nullptr;
  }
  
  void setUnit(unsigned int unit) {
    cUnit = unit;
  }

  T* getInsertionCell(int start, int end) {
    int mCellSpacing = cUnit / 2;
    int mCellStepsStart = (int) floor(start / (double) mCellSpacing);
    int mCellStepsEnd   = (int) floor(end   / (double) mCellSpacing);
    if (mCellStepsEnd - mCellStepsStart >= 2) {
      if (cBiggerSpace == nullptr) {
        cBiggerSpace = new SpatialContainer1D<T>();
        cBiggerSpace->setUnit(cUnit * 2);
      }
      return cBiggerSpace->getInsertionCell(start, end);
    }
    int mCellIndex = mCellStepsStart - cStepOffset;
    T* mCell;
   
    // If the container is currently empty
    if (cCells.empty()) {
      cStepOffset = mCellIndex;
      mCell = new T();
      cCells.push_back(mCell);

    // If the object is "behind" the first cell     
    } else if (mCellIndex < 0) {
      for (int i = mCellIndex; i < -1; i++) {
        cCells.insert(cCells.begin(), nullptr);
      }
      cStepOffset += mCellIndex;
      mCell = new T();
      cCells.insert(cCells.begin(), mCell);
     
    // If the object is "after" the last cell
    } else if (mCellIndex >= cCells.size()) {
      for (int i = cCells.size(); i < mCellIndex; i++) {
        cCells.push_back(nullptr);
      }
      mCell = new T();
      cCells.push_back(mCell);
     
    // The object is in an existing cell
    } else {
      mCell = cCells[mCellIndex];
      if (mCell == nullptr) {
        mCell = new T();
        cCells[mCellIndex] = mCell;
      }
    }
    return mCell;
  }

  std::vector<T*> getElements() {
    std::vector<T*> mResult;
    mResult.push_back(cCells.end(), cCells.begin(), cCells.end());
    if (cBiggerSpace != nullptr) {
      std::vector<T*> mBiggerElements = cBiggerSpace->getElements();
      mResult.insert(mResult.end(), mBiggerElements.begin(), mBiggerElements.end());
    }
    return mResult;
  }
  
  std::vector<T*> getElements(int start, int end) {
    int mCellSpacing = cUnit / 2;
    int mCellStepsStart = (int) floor(start / (double) mCellSpacing);
    int mCellStepsEnd   = (int) floor(end   / (double) mCellSpacing);
    int mRowIndexStart = std::max(0, (mCellStepsStart - cStepOffset) - 1);
    int mCellCount = cCells.size();
    int mRowIndexEnd   = std::min(mCellCount - 1, mCellStepsEnd - cStepOffset);

    std::vector<T*> mResult;
    for (int i = mRowIndexStart; i <= mRowIndexEnd; i++) {
      T* mCell = cCells[i];
      mResult.push_back(mCell);
    }

    if (cBiggerSpace != nullptr) {
      std::vector<T*> mBiggerElements = cBiggerSpace->getElements(start, end);
      mResult.insert(mResult.end(), mBiggerElements.begin(), mBiggerElements.end());
    }
    return mResult;
  }
  
  std::vector<T*> getAllCells() {
    std::vector<T*> mCells;
    mCells.insert(mCells.end(), cCells.begin(), cCells.end());
    if (cBiggerSpace != nullptr) {
      std::vector<T*> mBiggerElements = cBiggerSpace->getAllCells();
      mCells.insert(mCells.end(), mBiggerElements.begin(), mBiggerElements.end());
    }
    return mCells;
  }
  
  void removeElement(T* element) {
    for (unsigned int i = cCells.size() - 1; i >= 0; i--) {
      if (cCells[i] == element) {
        cCells->erase(cCells->begin() + i);
      }
    }
    
    if (cBiggerSpace != nullptr) {
      cBiggerSpace->removeElement(element);
    }
  }
 
  void clear() {
    cCells.clear();
  }
};

#endif
