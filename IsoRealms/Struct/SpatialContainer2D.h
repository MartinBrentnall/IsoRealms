/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <iostream>
#include <vector>

#include "IsoRealms/Literals.h"

#include "SpatialContainer1D.h"

namespace IsoRealms {
  template <typename T> class SpatialContainer2D {
    private:
    SpatialContainer1D<SpatialContainer1D<std::vector<T>>> cContainer;

    public:
    SpatialContainer2D() {
      cContainer.setUnit(8);
    }
      
    void add(int west, int east, int south, int north, T element) {
      SpatialContainer1D<std::vector<T>>* mRow = cContainer.getInsertionCell(south, north);
      std::vector<T>* mCell = mRow->getInsertionCell(west, east);
      mCell->push_back(element);
    }

    void remove(T element) {
      std::vector<SpatialContainer1D<std::vector<T>>*> mRows = cContainer.getAllCells();
      for (unsigned int i = 0; i < mRows.size(); i++) {
        if (mRows[i] != nullptr) {
          std::vector<std::vector<T>*> mCells = mRows[i]->getAllCells();
          for (unsigned int j = 0; j < mCells.size(); j++) {
            std::vector<T>* mCell = mCells[j];
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
    
    bool contains(int west, int east, int south, int north, T element) {
      std::vector<T> mNearbyElements = getElements(south, north, west, east);
      for (T mElement : mNearbyElements) {
        if (mElement == element) {
          return true;
        }
      }
      return false;
    }
    
    void clear() {
      std::vector<SpatialContainer1D<std::vector<T>>> mRows = cContainer.getAllElements();
      for (unsigned int i = 0; i < mRows.size(); i++) {
        std::vector<std::vector<T>> mCells = mRows[i].getAllElements();
        for (unsigned int j = 0; j < mCells.size(); j++) {
          mCells[j].clear();
        }
        mRows[i].clear();
      }
      cContainer.clear();
    }
  
    std::vector<T> getElements() {
      std::vector<T> mResult;
      std::vector<SpatialContainer1D<std::vector<T>>*> mRows = cContainer.getElements();
      for (unsigned int i = 0; i < mRows.size(); i++) {
        if (mRows[i] != nullptr) {
          std::vector<std::vector<T>*> mCells = mRows[i]->getElements();
          for (unsigned int j = 0; j < mCells.size(); j++) {
            if (mCells[j] != nullptr) {
              mResult.insert(mResult.begin(), mCells[j]->begin(), mCells[j]->end());
            }
          }
        }
      }
      return mResult;
    }
  
    std::vector<T> getElements(int x, int y) {
      return getElements(y, y, x, x);
    }
  
    std::vector<T> getElements(LiteralVertex& start, LiteralVertex& end) {
      int mSouth = std::min(start.y, end.y);
      int mNorth = std::max(start.y, end.y);
      int mWest  = std::min(start.x, end.x);
      int mEast  = std::max(start.x, end.x);
      return getElements(mSouth, mNorth, mWest, mEast);
    }
  
    std::vector<T> getElements(int south, int north, int west, int east) {
      std::vector<T> mResult;
      std::vector<SpatialContainer1D<std::vector<T>>*> mRows = cContainer.getElements(south, north);
      for (unsigned int i = 0; i < mRows.size(); i++) {
        if (mRows[i] != nullptr) {
          std::vector<std::vector<T>*> mCells = mRows[i]->getElements(west, east);
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
}
