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
#include "GroupAndCondition.h"

bool GroupAndCondition::isTrue() {
  for (int i = 0; i < cConditions.size(); i++) {
    if (!cConditions[i]->isTrue()) {
      return false;
    }
  }
  return true;
}

bool GroupAndCondition::canBeTrue(vector<int> onSwitches, vector<int> offSwitches) {
  for (int i = 0; i < cConditions.size(); i++) {
    if (!cConditions[i]->canBeTrue(onSwitches, offSwitches)) {
      return false;
    }
  }
  return true;
}

bool GroupAndCondition::isCompatibleWith(Condition* condition) {
  for (int i = 0; i < cConditions.size(); i++) {
    if (!cConditions[i]->isCompatibleWith(condition)) {
      return false;
    }
  }
  return true;
}

bool GroupAndCondition::contains(Condition* condition) {
  for (int i = 0; i < cConditions.size(); i++) {
    if (cConditions[i]->equals(*condition)) {
      return true;
    }
  }
  return false;
}

Condition* GroupAndCondition::invert() {
  GroupAndCondition* mInvertedCondition = new GroupAndCondition();
  for (int i = 0; i < cConditions.size(); i++) {
    Condition* mInvertedSubCondition = cConditions[i]->invert();
    mInvertedCondition->cConditions.push_back(mInvertedSubCondition);
  }
  return mInvertedCondition;
}

void GroupAndCondition::addCondition(Condition* condition) {
  // TODO: Should not be able to add the same condition twice.
  cConditions.push_back(condition);
}

int GroupAndCondition::size() {
  return cConditions.size();
}

bool GroupAndCondition::equals(GroupAndCondition& condition) {
  if (condition.size() != size()) {
    return false;
  }

  for (int i = 0; i < cConditions.size(); i++) {
    if (!(condition.contains(cConditions[i]))) {
      return false;
    }
  }
  return true;
}

void GroupAndCondition::debug() {
  for (int i = 0; i < cConditions.size(); i++) {
//    cConditions[i]->debug();
  }
}

bool GroupAndCondition::equals(Condition& condition) {
  return false;
}

void GroupAndCondition::write(FILE* w) {
  // TODO: Implement me!
}
