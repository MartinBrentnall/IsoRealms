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
#include "WallColumn.h"

WallColumn::WallColumn(int x, int y, int facing) {
  cX = x;
  cY = y;
  cFacing = facing;
  cCondition = NULL;
}

WallColumn::WallColumn(int x, int y, int facing, int lowa, int lowb, int higha, int highb, bool sp, bool pri) {
  cX = x;
  cY = y;
  cFacing = facing;
  cWallSections.push_back(WallSection(lowa, lowb, higha, highb, NULL));
  cSubtraction = sp;
  priority = pri;
  cCondition = NULL;
}

WallColumn::WallSection::WallSection(int lowa, int lowb, int higha, int highb, Condition* condition) {
  la = lowa;
  lb = lowb;
  ha = higha;
  hb = highb;
  flatTop = false;
  flatBottom = false;
  cCondition = condition;
}

bool WallColumn::WallSection::isAbove(int height) {
  return la >= height && lb >= height && ha >= height && hb >= height;
}

void WallColumn::WallSection::debug() {
  cout << "Low: " << la << ", " << lb << "   High: " << ha << ", " << hb << ":" << endl; 
  if (cCondition != NULL) {
//    cCondition->debug();
  } else {
    ////cout << "   No condition" << endl;
  }
}

int WallColumn::getX() {
  return cX;
}

int WallColumn::getY() {
  return cY;
}

bool WallColumn::hasSectionAbove(int height) {
  for (int i = 0; i < getSectionCount(); i++) {
    WallColumn::WallSection *mWallSection = getSection(i);
    if (mWallSection->isAbove(height)) {
      return true;
    }
  }
  return false;
}

void WallColumn::removeSectionsAbove(int height) {
  for (int i = 0; i < getSectionCount(); i++) {
    WallColumn::WallSection *mWallSection = getSection(i);
    if (mWallSection->isAbove(height)) {
      removeSection(i);
      i--;
    }
  }
}

void WallColumn::debug() {
  for (int i = 0; i < getSectionCount(); i++) {
    WallColumn::WallSection *mWallSection = getSection(i);
    mWallSection->debug();
  }
  if (getSectionCount() == 0) {
    cout << "EMPTY" << endl;
  }
}

bool WallColumn::WallSection::isHiddenBy(WallSection wallSection) {
  return la >= wallSection.la && lb >= wallSection.lb && ha <= wallSection.ha && hb <= wallSection.hb;
}

void WallColumn::removeSectionsHiddenBy(WallColumn wallSegment) {
  for (int i = 0; i < getSectionCount(); i++) {
    WallColumn::WallSection *mThisWallSection = getSection(i);
    for (int j = 0; j < wallSegment.getSectionCount(); j++) {
      WallColumn::WallSection *mThatWallSection = wallSegment.getSection(j);
      if (mThisWallSection->isHiddenBy(*mThatWallSection)) {

        // Remove hidden section.
        removeSection(i);
        i--;
        break;
      }
    }
  }
}

bool WallColumn::WallSection::isOverlapping(WallSection wallSection) {
  return ((ha >= wallSection.la && ha <= wallSection.ha) || (la >= wallSection.la && la <= wallSection.ha))
      && ((hb >= wallSection.lb && hb <= wallSection.hb) || (lb >= wallSection.lb && lb <= wallSection.hb));
}

bool WallColumn::WallSection::isInside(WallSection& section) {
  return (la > section.la && ha < section.ha) || (lb > section.lb && hb < section.hb);
}

bool WallColumn::WallSection::isCovering(WallSection& section) {
  return (section.la >= la && section.ha <= ha) && (section.lb >= lb && section.hb <= hb);
}

bool WallColumn::WallSection::isHigherThan(WallSection& section) {
  return (la > section.la && lb > section.lb) || (ha > section.ha && hb > section.hb);
}

int WallColumn::WallSection::testRelationWith(WallSection& section) {
  if (isInside(section))     {return IS_INSIDE;}
  if (isCovering(section))   {return IS_COVERING;}
  if (isHigherThan(section)) {return IS_HIGHER;}
  /* Must be */               return IS_LOWER;
}

void WallColumn::subtract(WallColumn& wallSegment) {
  for (int i = 0; i < getSectionCount(); i++) {
    WallColumn::WallSection* mThisSection = getSection(i);
    for (int j = 0; j < wallSegment.getSectionCount(); j++) {
      WallColumn::WallSection* mThatSection = wallSegment.getSection(j);
      bool mStartAgain = false;

      switch (mThatSection->testRelationWith(*mThisSection)) {
        case IS_INSIDE: { // Section is split in two
          WallColumn::WallSection mNewSegment(mThisSection->la, mThisSection->lb, mThatSection->la, mThatSection->lb, cCondition);
          mThisSection->la = mThatSection->ha;
          mThisSection->lb = mThatSection->hb;
          addSection(mNewSegment);
          mStartAgain = true;
          break;
        }
          
        case IS_COVERING: {// Section is removed
          removeSection(i);
          mStartAgain = true;
          break;
        }

        case IS_HIGHER: {// Section may become shorter at top
          mThisSection->ha = min(mThisSection->ha, mThatSection->la);
          mThisSection->hb = min(mThisSection->hb, mThatSection->lb);
          break;
        }

        case IS_LOWER: {// Section may become shorter at bottom
          mThisSection->la = max(mThisSection->la, mThatSection->ha);
          mThisSection->lb = max(mThisSection->lb, mThatSection->hb);
          break;
        }
      }

      if (mStartAgain) {
        i = 0;
        break;
      }
    }
  }
  
  for (int i = 0; i < getSectionCount(); i++) {
    WallColumn::WallSection* mThisSection = getSection(i);
  }
}

// This segment becomes the upper part of the split, whilst the lower half is returned.
// Each should be checked for validity afterwards.
WallColumn WallColumn::isSplit(WallColumn w) {
}

int WallColumn::getSectionCount() {
  return cWallSections.size();
}

Condition* WallColumn::WallSection::getCondition() {
  return cCondition;
}

WallColumn::WallSection* WallColumn::getSection(int index) {
  return &cWallSections[index];
}

bool WallColumn::isSubtraction() {
  return cSubtraction;
}

bool WallColumn::WallSection::isValid() {
  return la < ha || lb < hb;
}

void WallColumn::addSection(WallSection wallSection) {
  cWallSections.push_back(wallSection);
}

void WallColumn::removeSection(int index) {
  WallColumn::WallSection mToRemove = cWallSections[index];
  cWallSections[index] = cWallSections[getSectionCount() - 1];
  cWallSections[getSectionCount() - 1] = mToRemove;
  cWallSections.pop_back();
}

void WallColumn::addSegment(WallColumn wallSegment) {
  // Add sections from segment
  for (int i = 0; i < wallSegment.getSectionCount(); i++) {
    WallSection* mWallSection = wallSegment.getSection(i);
    addSection(*mWallSection);
  }

 // Merge joined sections together
  for (int i = 0; i < getSectionCount() - 1; i++) {
    WallSection* mWallSectionA = getSection(i);
    for (int j = i + 1; j < getSectionCount(); j++) {
      WallSection* mWallSectionB = getSection(j);
      if (mWallSectionA->isOverlapping(*mWallSectionB)) {

        // Remove section A and make section B into the new segment
        mWallSectionA->la = min(mWallSectionA->la, mWallSectionB->la);
        mWallSectionA->lb = min(mWallSectionA->lb, mWallSectionB->lb);
        mWallSectionA->ha = max(mWallSectionA->ha, mWallSectionB->ha);
        mWallSectionA->hb = max(mWallSectionA->hb, mWallSectionB->hb);

        removeSection(j);

        // Section A (i) has changed; do comparisons with section B again
//        j = i;
        i = 0;
        j = 0;
      }
    }
  }
}

int WallColumn::getDirection() {
  return cFacing;
}

bool WallColumn::isEmpty() {
  return getSectionCount() == 0;
}

bool WallColumn::equals(WallColumn* wallColumn) {
  if (getSectionCount() != wallColumn->getSectionCount()) {
    return false;
  }
  for (int i = 0; i < getSectionCount(); i++) {
    WallSection* mThisWallSection = getSection(i);
    WallSection* mThatWallSection = wallColumn->getSection(i);
    if (!mThisWallSection->equals(mThatWallSection)) {
      return false;
    }
  }
  return true;
}

bool WallColumn::WallSection::equals(WallSection* wallSection) {
  return la == wallSection->la
      && lb == wallSection->lb
      && ha == wallSection->ha
      && hb == wallSection->hb;
}

void WallColumn::setCondition(Condition* condition) {
  cCondition = condition;
  for (int i = 0; i < getSectionCount(); i++) {
    WallSection* mWallSection = getSection(i);
    mWallSection->cCondition = cCondition;
  }
}

bool WallColumn::excludes(int z, int heightA, int heightB) {
}


bool WallColumn::attemptMerge(WallColumn& wallColumn) {
  if (!equals(&wallColumn)) {
    return false;
  }
  if (cCondition != NULL) {
//    cCondition->addCondition(wallColumn.cCondition);
  } else if (wallColumn.cCondition != NULL) {
    cCondition = wallColumn.cCondition;
  }
  return true;
}
