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
#ifndef WALL_SEGMENT_H
#define WALL_SEGMENT_H

#include <math.h>
#include <iostream> // NOTE: For debug.
#include <vector>

#include "SpindizzyConstants.h"
#include "MiscFunctions.h"
#include "ColourScheme.h"
#include "Condition.h"

using namespace std; // TODO: Localise this.

/**
 * Represents a column of wall (which may or may not be split into several sections).
 */
class WallColumn:public SpindizzyConstants {
  private:
  static const int IS_HIGHER = 1;
  static const int IS_LOWER = 2;
  static const int IS_COVERING = 3;
  static const int IS_INSIDE = 4;

  public:

  /** True if this wall column is a subtraction. */
  bool cSubtraction;

  /** ? */
  bool priority;

  /** Face direction of the wall colummn. */
  int cFacing;

  /** X tile location of the wall column. */
  int cX;

  /** Y tile location of the wall column. */
  int cY;

  /** Condition on which this column should be present. */
  Condition* cCondition;

  /**
   * Represents a section (a single panel) within a wall section.
   */
  class WallSection {
    public:
    int la;
    int lb;
    bool flatBottom;
    int ha;
    int hb;
    bool flatTop;
    int floorHeight;
    Condition* cCondition;

    private:
    bool isCovering(WallSection&);
    bool isInside(WallSection&);
    bool isHigherThan(WallSection&);

    public:
    WallSection(int lowa, int lowb, int higha, int highb, Condition* condition);
    bool isHiddenBy(WallSection);
    bool isOverlapping(WallSection);
    void debug();
    Condition* getCondition();

    /**
     * Test the relation between this section and the specified one.
     * The result describes for example, that "This section IS_INSIDE the specified one"
     */
    int testRelationWith(WallSection&);
    bool equals(WallSection*);

    bool isAbove(int);
    bool isValid();
  };

  vector<WallSection> cWallSections;

  public:
  WallColumn(int, int, int);
  WallColumn(int, int, int, int, int, int, int, bool, bool);
  int getX();
  int getY();
  void addSegment(WallColumn);
  void removeSectionsHiddenBy(WallColumn);

  bool equals(WallColumn*);

  /**
   * Apply a condition on which this column should be present.  Without a
   * condition, the column is always present.
   */
  void setCondition(Condition* condition);

  /**
   * Subtract the specified wall segment from this one.
   * TODO: (I think; might be other way round)
   */
  void subtract(WallColumn&);
  bool combine(WallColumn);
  bool excludes(int, int, int);
  void removeSection(int index);
  void addSection(WallSection);
  bool hasSectionAbove(int);
  void removeSectionsAbove(int);
  WallColumn isSplit(WallColumn);
  WallColumn::WallSection* getSection(int index);

  /**
   * Get the face direction of this wall.
   *
   * @returns  Face direction of this wall.
   */
  int getDirection();

  /**
   * Determine whether this wall segment represents a subtraction.
   *
   * @returns  true if this section is a subtraction, otherwise false.
   */
  bool isSubtraction();

  /**
   * Determine whether the wall column has no sections in it.
   *
   * @returns  true if the column has no sections, otherwise false.
   */
  bool isEmpty();

  bool attemptMerge(WallColumn& wallColumn);

  /**
   * Get the number of sections in this wall.
   */
  int getSectionCount();
  void debug();
};

#endif
