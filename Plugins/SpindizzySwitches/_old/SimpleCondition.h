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
#ifndef SIMPLE_CONDITION_H
#define SIMPLE_CONDITION_H

#include <vector>

#include "SpindizzyConstants.h"
#include "SwitchStates.h"
#include "Condition.h"

class SimpleCondition:public SpindizzyConstants, Condition {
  private:
  /** Reference to global switch states on which to check conditions. */
  static SwitchStates* cSwitchStates;

  /** If true, the condition is negated. */
  bool cNot;

  /** A switch that must be active. */
  int cSymbol;

  public:
  /**
   * Set a pointer to the global switch states on which conditions are tested.
   */
  static void setSwitchStates(SwitchStates* switchStates);

  /**
   * Construct a new condition.
   *
   * @param int   Switch to be active in satisfying this condition.
   * @param int   Another switch to be active in satisfying this condition (optional).
   * @param bool  true to negate the condition (specified switches must be inactive).
   */
  SimpleCondition(int symbolA, bool invert);

  /**
   * Test this condition against the specified one for equality.  Two
   * conditions are equal when they are both garaunteed to always return
   * the same result from isTrue().
   *
   * @param Condition*  The condition to test for equality with this one.
   * @returns           true if both conditions are equal, othewise false.
   */
  bool equals(Condition& condition); // TODO: equals operator

  /**
   * Test whether both this conditions and the specified one can be true
   * simultaneously.
   *
   * @param Condition*  The condition to test for compatibility with this one.
   * @returns           true if the conditions are not mutually exclusive.
   */
  bool isCompatibleWith(Condition* condition);

  /**
   *
   */
  void write(FILE* w);

  Condition* invert();
  bool isTrue();
  bool canBeTrue(vector<int>, vector<int>);
  void debug();
};

#endif
