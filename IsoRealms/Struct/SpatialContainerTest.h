/*
 * Copyright 2023 Martin Brentnall
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
#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <vector>

#include "IsoRealms/Utils.h"

namespace IsoRealms {
  template <class TYPE> class SpatialContainerTest {
    private:
    class Entry {
      private:
      int cXStart;
      int cXEnd;
      int cYStart;
      int cYEnd;
      TYPE cElement;

      public:
      Entry(int xStart, int xEnd, int yStart, int yEnd, TYPE element) :
              cXStart(xStart),
              cXEnd(xEnd),
              cYStart(yStart),
              cYEnd(yEnd),
              cElement(std::move(element)) {
      }

      Entry(const Entry& entry) :
              cXStart(entry.cXStart),
              cXEnd(entry.cXEnd),
              cYStart(entry.cYStart),
              cYEnd(entry.cYEnd),
              cElement(entry.cElement) {
      }

      TYPE& element() {
        return cElement;
      }

      int getXStart() {
        return cXStart;
      }

      int getYStart() {
        return cYStart;
      }

      int getXEnd() {
        return cXEnd;
      }

      int getYEnd() {
        return cYEnd;
      }
    };

    class Cell {
      private:
      std::vector<Entry*> cDefEntries;

      public:
      Cell() {
      }

      void add(Entry* entry) {
        cDefEntries.emplace_back(entry);
      }

      void remove(Entry* entry) {
        Utils::removeElement(cDefEntries, entry);
      }

      void clear() {
        cDefEntries.clear();
      }

      int size() {
        return cDefEntries.size();
      }

      Entry*& operator[](int index) {
        return cDefEntries[index];
      }
    };

    class Row {
      private:
      SpatialContainerTest<TYPE>* cDefParent;
      int cDefCellOffset;
      std::vector<Cell> cDefCells;

      public:
      Row(SpatialContainerTest<TYPE>* parent) :
              cDefParent(parent),
              cDefCellOffset(0) {
      }

      void add(int start, int end, Entry* entry) {
        int mStartCell = start / cDefParent->cDefCellSize;
        int mEndCell   = end   / cDefParent->cDefCellSize;

        if (cDefCells.empty()) {
          cDefCells.emplace_back();
          cDefCellOffset = mStartCell;
        }

        for (int i = cDefCellOffset + cDefCells.size(); i <= mEndCell; i++) {
          cDefCells.emplace_back();
        }
        for (int i = mStartCell; i < cDefCellOffset; i++) {
          cDefCells.insert(cDefCells.begin(), Cell());
        }
        cDefCellOffset = std::min(cDefCellOffset, mStartCell);

        int mStartIndex = mStartCell - cDefCellOffset;
        int mEndIndex   = mEndCell   - cDefCellOffset;
        for (int i = mStartIndex; i <= mEndIndex; i++) {
          if (i >= static_cast<int>(cDefCells.size())) {
            std::cout << "TODO: You fucked up! (cells: " << cDefCells.size() << "   cell range: " << mStartCell << " to " << mEndCell << "   insertion range: " << mStartIndex << " to " << mEndIndex << "   offset: " << cDefCellOffset << ")" << std::endl;
          }
          cDefCells[i].add(entry);
        }
      }

      int getCellOffset() {
        return cDefCellOffset;
      }

      int size() {
        return cDefCells.size();
      }

      Cell& operator[](int index) {
        return cDefCells[index];
      }
    };

    int cDefCellSize;
    int cDefRowsOffset;
    std::vector<Row> cDefRows;
    std::set<std::unique_ptr<Entry>> cDefEntries;
    std::mutex cModificationMutex;                      /// Mutex controls modification of this container

    public:
    class Search {
      private:
      SpatialContainerTest<TYPE>* cDefParent;
      int cDefXStart;
      int cDefXEnd;
      int cDefYStart;
      int cDefYEnd;

      public:
      class Iterator {
        private:
        Search* cDefParent;
        int cRowIndex;
        int cCellIndex;
        int cEntryIndex;

        void next() {
          int mSearchYEndIndex = cDefParent->cDefYEnd / cDefParent->cDefParent->cDefCellSize - cDefParent->cDefParent->cDefRowsOffset;
          while (cRowIndex < std::min(static_cast<int>(cDefParent->cDefParent->cDefRows.size()), mSearchYEndIndex + 1)) {
            int mSearchXEndIndex = cDefParent->cDefXEnd / cDefParent->cDefParent->cDefCellSize - cDefParent->cDefParent->cDefRows[cRowIndex].getCellOffset();
            while (cCellIndex < std::min(cDefParent->cDefParent->cDefRows[cRowIndex].size(), mSearchXEndIndex + 1)) {
              while (cEntryIndex < cDefParent->cDefParent->cDefRows[cRowIndex][cCellIndex].size()) {
                int mElementYStartIndex = cDefParent->cDefParent->cDefRows[cRowIndex][cCellIndex][cEntryIndex]->getYStart() / cDefParent->cDefParent->cDefCellSize - cDefParent->cDefParent->cDefRowsOffset;
                int mElementXStartIndex = cDefParent->cDefParent->cDefRows[cRowIndex][cCellIndex][cEntryIndex]->getXStart() / cDefParent->cDefParent->cDefCellSize - cDefParent->cDefParent->cDefRows[cRowIndex].getCellOffset();
                int mSearchYStartIndex = cDefParent->cDefYStart / cDefParent->cDefParent->cDefCellSize - cDefParent->cDefParent->cDefRowsOffset;
                int mSearchXStartIndex = cDefParent->cDefXStart / cDefParent->cDefParent->cDefCellSize - cDefParent->cDefParent->cDefRows[cRowIndex].getCellOffset();
                if (cCellIndex == std::max(mSearchXStartIndex, mElementXStartIndex) && cRowIndex == std::max(mSearchYStartIndex, mElementYStartIndex)) {
                  return;
                }
                cEntryIndex++;
              }
              cCellIndex++;
              cEntryIndex = 0;
            }
            cRowIndex++;
            cCellIndex = std::max(0, cDefParent->cDefXStart / cDefParent->cDefParent->cDefCellSize - (cRowIndex < static_cast<int>(cDefParent->cDefParent->cDefRows.size()) ? cDefParent->cDefParent->cDefRows[cRowIndex].getCellOffset() : 0));
          }
        }

        public:
        Iterator(Search* parent, int row) :
                cDefParent(parent),
                cRowIndex(std::clamp(row, 0, static_cast<int>(cDefParent->cDefParent->cDefRows.size()))),
                cCellIndex(std::max(0, cDefParent->cDefXStart / cDefParent->cDefParent->cDefCellSize - (cRowIndex < static_cast<int>(cDefParent->cDefParent->cDefRows.size()) ? cDefParent->cDefParent->cDefRows[cRowIndex].getCellOffset() : 0))),
                cEntryIndex(0) {
          next();
        }

        bool operator!=(const Iterator& other) {
          return cRowIndex != other.cRowIndex || cCellIndex != other.cCellIndex || cEntryIndex != other.cEntryIndex;
        }

        Iterator& operator++() {
          cEntryIndex++;
          next();
          return *this;
        }

        TYPE& operator*() {
          return cDefParent->cDefParent->cDefRows[cRowIndex][cCellIndex][cEntryIndex]->element();
        }
      };

      Search(SpatialContainerTest<TYPE>* parent, int xStart, int xEnd, int yStart, int yEnd) :
              cDefParent(parent),
              cDefXStart(xStart),
              cDefXEnd(xEnd),
              cDefYStart(yStart),
              cDefYEnd(yEnd) {
      }

      Iterator begin() {
        return Iterator(this, cDefYStart / cDefParent->cDefCellSize - cDefParent->cDefRowsOffset);
      }

      Iterator end() {
        return Iterator(this, (cDefYEnd / cDefParent->cDefCellSize - cDefParent->cDefRowsOffset) + 1);
      }

      bool contains(TYPE element) {
        for (TYPE mElement : *this) {
          if (mElement == element) {
            return true;
          }
        }
        return false;
      }

      void remove(Entry* entry) {
        int mSearchYStartIndex = cDefYStart / cDefParent->cDefCellSize - cDefParent->cDefRowsOffset;
        int mSearchYEndIndex   = cDefYEnd   / cDefParent->cDefCellSize - cDefParent->cDefRowsOffset;
        for (int y = mSearchYStartIndex; y <= mSearchYEndIndex; y++) {
          int mSearchXStartIndex = cDefXStart / cDefParent->cDefCellSize - cDefParent->cDefRows[y].getCellOffset();
          int mSearchXEndIndex   = cDefXEnd   / cDefParent->cDefCellSize - cDefParent->cDefRows[y].getCellOffset();
          for (int x = mSearchXStartIndex; x <= mSearchXEndIndex; x++) {
            cDefParent->cDefRows[y][x].remove(entry);
          }
        }
      }

      void clear() {
        std::vector<TYPE*> mElementsToRemove;
        for (TYPE& mElement : *this) {
          mElementsToRemove.emplace_back(&mElement);
        }

        for (TYPE* mElement : mElementsToRemove) {
          cDefParent->remove(*mElement);
        }
      }
    };

    class Iterator {
      private:
      typename std::set<std::unique_ptr<Entry>>::iterator cDefIterator;

      public:
      Iterator(typename std::set<std::unique_ptr<Entry>>::iterator iterator) :
              cDefIterator(iterator) {
      }

      bool operator!=(const Iterator& other) {
        return cDefIterator == other.cDefIterator;
      }

      Iterator& operator++() {
        cDefIterator++;
        return *this;
      }

      const TYPE& operator*() {
        return (*cDefIterator)->element();
      }
    };

    SpatialContainerTest<TYPE>() :
            cDefCellSize(8),
            cDefRowsOffset(0) {
    }

    void add(int xStart, int xEnd, int yStart, int yEnd, TYPE element) {
      int mStartRow = yStart / cDefCellSize;
      int mEndRow   = yEnd   / cDefCellSize;

      cModificationMutex.lock();
      if (cDefRows.empty()) {
        cDefRows.emplace_back(this);
        cDefRowsOffset = mStartRow;
      }

      for (int i = mStartRow; i < cDefRowsOffset; i++) {
        cDefRows.insert(cDefRows.begin(), this);
      }
      for (int i = cDefRowsOffset + cDefRows.size(); i <= mEndRow; i++) {
        cDefRows.emplace_back(this);
      }
      cDefRowsOffset = std::min(cDefRowsOffset, mStartRow);

      Entry* mEntry = cDefEntries.emplace(std::make_unique<Entry>(xStart, xEnd, yStart, yEnd, std::move(element))).first->get();

      int mStartIndex = mStartRow - cDefRowsOffset;
      int mEndIndex   = mEndRow   - cDefRowsOffset;
      for (int i = mStartIndex; i <= mEndIndex; i++) {
        cDefRows[i].add(xStart, xEnd, mEntry);
      }
      cModificationMutex.unlock();
    }

    Search search(int xStart, int xEnd, int yStart, int yEnd) {
      return Search(this, xStart, xEnd, yStart, yEnd);
    }

    Search search(int x, int y) {
      return search(x, x, y, y);
    }

    bool contains(TYPE& element) {
      for (const std::unique_ptr<Entry>& mEntry : cDefEntries) {
        if (element == mEntry->element()) {
          return true;
        }
      }
      return false;
    }

    bool remove(TYPE& element) {
      typename std::set<std::unique_ptr<Entry>>::iterator mIterator = cDefEntries.begin();
      bool mRemoved = false;
      while (mIterator != cDefEntries.end()) {
        if (element == (*mIterator)->element()) {
          cModificationMutex.lock();
          search((*mIterator)->getXStart(), (*mIterator)->getXEnd(), (*mIterator)->getYStart(), (*mIterator)->getYEnd()).remove((*mIterator).get());
          mIterator = cDefEntries.erase(mIterator);
          cModificationMutex.unlock();
          mRemoved = true;
        } else {
          mIterator++;
        }
      }
      return mRemoved;
    }

    Iterator begin() {
      return Iterator(cDefEntries.begin());
    }

    Iterator end() {
      return Iterator(cDefEntries.end());
    }

    void clear() {
      cModificationMutex.lock();
      cDefEntries.clear();
      cDefRows.clear();
      cDefRowsOffset = 0;
      cModificationMutex.unlock();
    }
  };
}
