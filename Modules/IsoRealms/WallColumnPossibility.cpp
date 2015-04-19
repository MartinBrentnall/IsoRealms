/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "WallColumnPossibility.h"

WallColumnPossibility::WallColumnPossibility() {
  cCondition = NULL;
}

WallColumnPossibility::WallColumnPossibility(WallColumn* wallColumn, Condition* condition) {
  cCondition = condition != NULL ? new Condition(*condition) : NULL;
  cSegments.push_back(new WallColumn(*wallColumn));
}

WallColumnPossibility::WallColumnPossibility(WallColumnPossibility* wallColumn, Condition* condition) {
  cCondition = condition != NULL ? new Condition(*condition) : NULL;
  for (unsigned int i = 0; i < wallColumn->cSegments.size(); i++) {
    cSegments.push_back(new WallColumn(*wallColumn->cSegments[i]));
  }
}

WallColumnPossibility* WallColumnPossibility::split(Condition* condition) {
  if (cCondition != NULL) {
    std::vector<Condition*> mSplitCondition = cCondition->split(condition);
    if (mSplitCondition.size() == 2) {
      delete cCondition;
      cCondition = mSplitCondition[0];
      return new WallColumnPossibility(this, mSplitCondition[1]);
    }
  } else if (condition != NULL) {
    cCondition = new Condition(*condition);
    Condition* mNegatedCondition = cCondition->negate();
    return new WallColumnPossibility(this, mNegatedCondition);
  }
  return NULL;
}

bool WallColumnPossibility::isCompatibleWith(Condition* condition) {
  return cCondition != NULL ? cCondition->isCompatibleWith(condition)
       : condition != NULL  ? !condition->isAbsolute() || condition->isTrue()
       :                      true;
}

std::vector<WallColumn*> WallColumnPossibility::getSections() {
  return cSegments;
}

void WallColumnPossibility::applyOverlapping(WallColumnPossibility* wallColumn) {
  std::vector<int> mIndicesToRemove;
  for (unsigned int i = 0; i < wallColumn->cSegments.size(); i++) {
    WallColumn* mWallColumn = wallColumn->cSegments[i];
    for (unsigned int j = 0; j < cSegments.size(); j++) {
      switch (cSegments[j]->getRelationWith(mWallColumn)) {
        case WallColumn::INSIDE: {
          WallColumn* mSplitColumn = new WallColumn(*cSegments[j]);
          mSplitColumn->lowerTop(mWallColumn);
          cSegments[j]->raiseBottom(mWallColumn);
          if (mSplitColumn->isAddition()) {
            cSegments.push_back(mSplitColumn);
          } else {
            delete mSplitColumn;
          }
          if (!cSegments[j]->isAddition()) {
            mIndicesToRemove.push_back(j);
          }
          break;
        }
        
        case WallColumn::COVERING: {
          mIndicesToRemove.push_back(j);
          break;
        }

        case WallColumn::HIGHER: {
          if (!cSegments[j]->lowerTop(mWallColumn)) {
            mIndicesToRemove.push_back(j);
          }
          break;
        }

        case WallColumn::LOWER: {
          if (!cSegments[j]->raiseBottom(mWallColumn)) {
            mIndicesToRemove.push_back(j);
          }
          break;
        }
      }
    }
  }
  for (int j = mIndicesToRemove.size() - 1; j >= 0; j--) {
    delete cSegments[mIndicesToRemove[j]];
    cSegments.erase(cSegments.begin() + mIndicesToRemove[j]);
  }
  mIndicesToRemove.clear();
}

bool WallColumnPossibility::empty() {
  return cSegments.empty();
}

bool WallColumnPossibility::isSubtraction() {
  if (!empty()) {
    return !cSegments[0]->isAddition();
  }
  return false;
}

void WallColumnPossibility::convertToAddition() {
  for (unsigned int i = 0; i < cSegments.size(); i++) {
    cSegments[i]->convertToAddition();
  }
}

void WallColumnPossibility::unite(WallColumnPossibility* wallColumn) {
  for (unsigned int i = 0; i < wallColumn->cSegments.size(); i++) {
    cSegments.push_back(new WallColumn(*wallColumn->cSegments[i]));
  }

  for (int i = 0; i < (signed int) (cSegments.size() - 1); i++) {
    for (unsigned int j = i + 1; j < cSegments.size(); j++) {
      if (cSegments[i]->unite(cSegments[j])) {
        delete cSegments[j];
        cSegments.erase(cSegments.begin() + j--);
      }
    }
  }
}

Condition* WallColumnPossibility::getCondition() {
  return cCondition != NULL ? new Condition(*cCondition) : NULL;
}

void WallColumnPossibility::shaveTop(int height, Condition* condition) {
  for (int i = cSegments.size() - 1; i >= 0; i--) {
    // TODO: This should not only be based on the start bottom height, but also the end bottom height aswell.
    int mWallBottom = cSegments[i]->getBottomHeightStart();
    if (mWallBottom >= height && condition != nullptr && !condition->isCompatibleWith(cCondition)) {
      delete cSegments[i];
      cSegments.erase(cSegments.begin() + i);
    }
  }
}

void WallColumnPossibility::removeHiddenSections(WallColumnPossibility* wallMask) {
  for (int i = cSegments.size() - 1; i >= 0; i--) {
    for (unsigned int j = 0; j < wallMask->cSegments.size(); j++) {
      if (wallMask->cSegments[j]->isCovering(cSegments[i])) {
//        delete cSegments[i]; TODO: This causes a crash (double-free)...  need to debug why this is NULL or invalid (it doesn't crash in debug mode though)
        cSegments.erase(cSegments.begin() + i);
      }
    }
  }
}

void WallColumnPossibility::debug() {
  std::cout << "Column has " << cSegments.size() << " segments for: ";
  if (cCondition != NULL) {
    cCondition->debug();
  } else {
    std::cout << "(true)" << std::endl;
  }
  for (unsigned int i = 0; i < cSegments.size(); i++) {
    std::cout << "   Segment " << i << ": ";
    cSegments[i]->debug();
  }
}

WallColumnPossibility::~WallColumnPossibility() {
  for (unsigned int i = 0; i < cSegments.size(); i++) {
    delete cSegments[i];
  }
  cSegments.clear();
  delete cCondition;
}


