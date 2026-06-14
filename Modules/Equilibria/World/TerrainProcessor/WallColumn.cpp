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
#include "WallColumn.h"

namespace IsoRealms::Equilibria {
  WallColumn::WallColumn() {
  }

  WallColumn::WallColumn(WallSegment* segment, std::optional<Condition>& condition) :
            cCondition(condition) {
    cSegments.emplace_back(std::make_unique<WallSegment>(*segment));
  }

  WallColumn::WallColumn(WallColumn* column, std::optional<Condition>& condition) :
            cCondition(condition) {
    for (unsigned int i = 0; i < column->cSegments.size(); i++) {
      cSegments.emplace_back(std::make_unique<WallSegment>(*column->cSegments[i]));
    }
  }

  std::unique_ptr<WallColumn> WallColumn::split(std::optional<Condition>& condition) {
    if (cCondition.has_value()) {
      std::vector<Condition> mSplitCondition = cCondition->split(condition);
      if (mSplitCondition.size() == 2) {
        cCondition = std::make_optional<Condition>(mSplitCondition[0]);
        std::optional<Condition> mOtherSplitCondition(mSplitCondition[1]);
        return std::make_unique<WallColumn>(this, mOtherSplitCondition);
      }
    } else if (condition.has_value()) {
      cCondition = std::make_optional<Condition>(*condition);
      std::optional<Condition> mNegatedCondition = cCondition->negate();
      return std::make_unique<WallColumn>(this, mNegatedCondition);
    }
    return nullptr;
  }

  bool WallColumn::isCompatibleWith(std::optional<Condition>& condition) {
    return cCondition.has_value() ? cCondition->isCompatibleWith(condition)
         : condition.has_value()  ? !condition->isAbsolute() || condition->isTrue()
         :                          true;
  }

  std::vector<std::unique_ptr<WallSegment>>& WallColumn::getSections() {
    return cSegments;
  }

  void WallColumn::applyOverlapping(WallColumn* column) {
    std::vector<int> mIndicesToRemove;
    for (unsigned int i = 0; i < column->cSegments.size(); i++) {
      WallSegment* mSegment = column->cSegments[i].get();
      for (unsigned int j = 0; j < cSegments.size(); j++) {
        switch (cSegments[j]->getRelationWith(mSegment)) {
          case WallSegment::Relation::INSIDE: {
            std::unique_ptr<WallSegment> mSplitSegment = std::make_unique<WallSegment>(*cSegments[j]);
            mSplitSegment->lowerTop(mSegment);
            cSegments[j]->raiseBottom(mSegment);
            if (mSplitSegment->isAddition()) {
              cSegments.emplace_back(std::move(mSplitSegment));
            }
            if (!cSegments[j]->isAddition()) {
              mIndicesToRemove.push_back(j);
            }
            break;
          }
          
          case WallSegment::Relation::COVERING: {
            mIndicesToRemove.push_back(j);
            break;
          }

          case WallSegment::Relation::HIGHER: {
            if (!cSegments[j]->lowerTop(mSegment)) {
              mIndicesToRemove.push_back(j);
            }
            break;
          }

          case WallSegment::Relation::LOWER: {
            if (!cSegments[j]->raiseBottom(mSegment)) {
              mIndicesToRemove.push_back(j);
            }
            break;
          }
        }
      }
    }
    for (int j = mIndicesToRemove.size() - 1; j >= 0; j--) {
      cSegments.erase(cSegments.begin() + mIndicesToRemove[j]);
    }
    mIndicesToRemove.clear();
  }

  bool WallColumn::empty() {
    return cSegments.empty();
  }

  bool WallColumn::isSubtraction() {
    if (!empty()) {
      return !cSegments[0]->isAddition();
    }
    return false;
  }

  void WallColumn::convertToAddition() {
    for (unsigned int i = 0; i < cSegments.size(); i++) {
      cSegments[i]->convertToAddition();
    }
  }

  void WallColumn::unite(std::unique_ptr<WallColumn>& column) {
    for (unsigned int i = 0; i < column->cSegments.size(); i++) {
      cSegments.emplace_back(std::make_unique<WallSegment>(*column->cSegments[i]));
    }

    for (int i = 0; i < (signed int) (cSegments.size() - 1); i++) {
      for (unsigned int j = i + 1; j < cSegments.size(); j++) {
        if (cSegments[i]->unite(cSegments[j].get())) {
          cSegments.erase(cSegments.begin() + j--);
        }
      }
    }
  }

  std::optional<Condition>& WallColumn::getCondition() {
    return cCondition;
  }

  void WallColumn::shaveTop(int height, std::optional<Condition>& condition) {
    for (int i = cSegments.size() - 1; i >= 0; i--) {
      // TODO: This should not only be based on the start bottom height, but also the end bottom height aswell.
      int mWallBottom = cSegments[i]->getBottomStart();
      if (mWallBottom >= height && condition.has_value() && !condition->isCompatibleWith(cCondition)) {
        cSegments.erase(cSegments.begin() + i);
      }
    }
  }

  void WallColumn::removeHiddenSections(WallColumn* wallMask) {
    for (int i = cSegments.size() - 1; i >= 0; i--) {
      for (unsigned int j = 0; j < wallMask->cSegments.size(); j++) {
        if (wallMask->cSegments[j]->isCovering(cSegments[i].get())) {
          cSegments.erase(cSegments.begin() + i);
          break;
        }
      }
    }
  }

  void WallColumn::debug() {
    std::cout << "Column has " << cSegments.size() << " segments for: ";
    if (cCondition.has_value()) {
      cCondition->debug();
    } else {
      std::cout << "(true)" << std::endl;
    }
    for (unsigned int i = 0; i < cSegments.size(); i++) {
      std::cout << "   Segment " << i << ": ";
      cSegments[i]->debug();
    }
  }
}
