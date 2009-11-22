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
#ifndef CONDITION_H
#define CONDITION_H

#include <stdio.h>
#include <vector>

using namespace std;

class Condition {
  public:

  /**
   * Determines whether this condition is satisfied by the global switch states.
   */
  virtual bool isTrue() = 0;

  /**
   * Determines whether this condition can be true when the specified switches
   * are active and inactive respectively.
   *
   * @param vector<int>  Active switches.
   * @param vector<int>  Inactive switches.
   */
  virtual bool canBeTrue(vector<int>, vector<int>) = 0;

  /**
   * Test whether both this conditions and the specified one can be true
   * simultaneously.
   *
   * @param Condition*  The condition to test for compatibility with this one.
   * @returns           true if the conditions are not mutually exclusive.
   */
  virtual bool isCompatibleWith(Condition*) = 0;

  /**
   * Return a negated version of this condition.
   */
  virtual Condition* invert() = 0;

  /**
   * Write the condition to a file.
   */
  virtual void write(FILE* w) = 0;

  virtual bool equals(Condition& condition) = 0;
};

#endif
