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
#ifndef WALL_COLUMN_H
#define WALL_COLUMN_H

#include <IsoRealms/MiscFunctions.h>

#include <iostream>

class WallColumn {
  private:
  int cBottomHeightStart;
  int cBottomHeightEnd;
  int cTopHeightStart;
  int cTopHeightEnd;

  bool isInside(WallColumn*);
  bool isHigher(WallColumn*);
  bool isOverlapping(WallColumn*);

  public:

  enum Relation {

    /**
     * Tested wall section is inside this one.  A subtraction should cause
     * this wall column to split into two sections.
     */
    INSIDE,
    
    /**
     * Tested wall section completely overlaps this one.  A subtraction should
     * completely erase this wall column.
     */
    COVERING,
    
    /**
     * Tested wall section is higher than this one.  A subtraction may reduced
     * the height at the top of this one.
     */
    HIGHER,
    
    /**
     * Tested wall section is lower than this one.  A subtraction may increase
     * the height at the bottom of this one.
     */
    LOWER
  };

  /**
   * Construct a new wall column.
   * 
   * @param int  Bottom height start.
   * @param int  Bottom height end.
   * @param int  Top height start.
   * @param int  Top height end.
   */
  WallColumn(int, int, int, int);
  
  int getBottomHeightStart();
  int getBottomHeightEnd();
  int getTopHeightStart();
  int getTopHeightEnd();

  bool isCovering(WallColumn*);

  bool isAddition();

  /**
   * Attempt to unite the specified wall column with this one.  On failure, the
   * wall column will remain unchanged.
   * 
   * @param WallColumn*  The wall column to unite with this one.
   * @returns  True for success, false for failure.
   */
  bool unite(WallColumn*);

  /**
   * Test how this wall column is related to the specified one.
   * 
   * @param WallColumn*  The wall column to test.
   * @returns  How the wall column is related to this one.
   */
  Relation getRelationWith(WallColumn*);

  /**
   * TODO
   * 
   * @returns  True if the resulting wall column is an addition, or false if
   *           the resulting wall column is a subtraction (or nothing).
   */
  bool raiseBottom(WallColumn*);

  /**
   * TODO
   * 
   * @returns  True if the resulting wall column is an addition, or false if
   *           the resulting wall column is a subtraction (or nothing).
   */
  bool lowerTop(WallColumn*);

  /**
   * Make sure this column is an addition and not a subtraction.  This is
   * useful when we wish to use a subtraction to subtract from another
   * wall.
   */
  void convertToAddition();
};

#endif
