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
#include "SimpleCondition.h"

SwitchStates* SimpleCondition::cSwitchStates = NULL;

void SimpleCondition::setSwitchStates(SwitchStates* switchStates) {
  cSwitchStates = switchStates;
}

SimpleCondition::SimpleCondition(int symbol, bool invert) {
  cSymbol = symbol;
  cNot = invert;
}

bool SimpleCondition::isTrue() {
  if (cSwitchStates != NULL) {
    return cSwitchStates->isActive(cSymbol) != cNot;
  }
  return false;
}

bool SimpleCondition::canBeTrue(vector<int> onSwitches, vector<int> offSwitches) {
  vector<int> mSwitchesToTest = cNot ? onSwitches : offSwitches;
  for (int i = 0; i < mSwitchesToTest.size(); i++) {
    if (mSwitchesToTest[i] == cSymbol) {
      return false;
    }
  }
  return true;
}

bool SimpleCondition::isCompatibleWith(Condition* condition) {
  vector<int> mOnSwitches;
  vector<int> mOffSwitches;
  vector<int> mSwitchToPass = cNot ? mOffSwitches : mOnSwitches;
  mSwitchToPass.push_back(cSymbol);
  return condition->canBeTrue(mOnSwitches, mOffSwitches);
}

bool SimpleCondition::equals(Condition& condition) {
  vector<int> mOnSwitches;
  vector<int> mOffSwitches;
  vector<int> mSwitchToPass = cNot ? mOffSwitches : mOnSwitches;
  mSwitchToPass.push_back(cSymbol);
//  return condition.isExclusively(mOnSwitches, mOffSwitches);
  return true;
}

void SimpleCondition::write(FILE *w) {
  fprintf(w, "CONDITION NOT %d SYMBOL_A %d", cNot ? 1 : 0, cSymbol);
}

Condition* SimpleCondition::invert() {
  return NULL;
}

void SimpleCondition::debug() {
  cout << "   Requires: " << cNot << ", " << cSymbol << endl;
}
