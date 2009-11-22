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
#ifndef GROUP_AND_CONDITION_H
#define GROUP_AND_CONDITION_H

#include <vector>

#include "Condition.h"

using namespace std;

/**
 * A GroupAndCondition represents a collection of conditions, all of which must
 * be met to satisfy the result.
 */
class GroupAndCondition:public Condition {
  private:

  /** Conditions to be met. */
  vector<Condition*> cConditions;

  /**
   * Test whether the specified condition is one of the conditions to be
   * satisfied by this condition.
   *
   * @param Condition*  The condition to test.
   * @returns           true if this condition contains the specified one.
   */
  bool contains(Condition* condition);

  public:
  /**
   * Add a condition to be met by this condition.
   */
  void addCondition(Condition* condition);

  /**
   * Returns the number of conditions to be satisfied by this condition.
   *
   * @returns  The number of individual conditions to be satisfied.
   */
  int size();

  /**
   * Test this condition against the specified one for equality.  Two
   * conditions are equal when they are both garaunteed to return the same
   * result from isTrue().
   *
   * @param GroupAndCondition&  The condition to test for equality with this one.
   * @return                 true if both conditions are equal, othewise false.
   */
  bool equals(GroupAndCondition& condition); // TODO Replace with operator overload

  /**************************************************************************\
   * Implemented methods of Condition                                       *
  \**************************************************************************/
  bool isTrue();
  bool canBeTrue(vector<int>, vector<int>);
  bool isCompatibleWith(Condition* condition);
  Condition* invert();
  bool equals(Condition& condition);
  void write(FILE* w);

  void debug();
};

#endif
