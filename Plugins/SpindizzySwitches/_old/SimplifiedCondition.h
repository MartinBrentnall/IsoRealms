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
#ifndef SIMPLIFIED_CONDITION_H
#define SIMPLIFIED_CONDITION_H

#include <vector>

#include "SwitchStates.h"

using namespace std;

class SimplifiedCondition {
  private:

  /** Reference to global switch states on which to check conditions. */
  static SwitchStates* cSwitchStates;

  vector<SimplifiedCondition> cConditions;
  vector<char> cSwitches;
  int cType;

  void removeCondition(int);

  void setConstant(bool);

  public:
  static const int NONE = 1;
  static const int NOT = 2;
  static const int AND = 3;
  static const int OR = 4;
  static const int NAND = 5;
  static const int NOR = 6;
  static const int ALWAYS_FALSE = 7;
  static const int ALWAYS_TRUE = 8;

  SimplifiedCondition(int gate);

  void addSwitch(int aSwitch);

  void addCondition(SimplifiedCondition condition);

  void simplify();

  void debug();
};

#endif


