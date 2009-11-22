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
#include "SimplifiedCondition.h"

SimplifiedCondition::SimplifiedCondition(int type) {
  cType = type;
}

void SimplifiedCondition::addSwitch(int aSwitch) {
  cSwitches.push_back(aSwitch);
}

void SimplifiedCondition::addCondition(SimplifiedCondition condition) {
  cConditions.push_back(condition);
}

void SimplifiedCondition::removeCondition(int index) {
  for (int i = index; i < cConditions.size() - 1; i++) {
    SimplifiedCondition mTemp = cConditions[i];
    cConditions[i] = cConditions[i + 1];
    cConditions[i + 1] = mTemp;
  }
  cConditions.pop_back();
}

void SimplifiedCondition::setConstant(bool value) {
  int mGCCFix = ALWAYS_FALSE;
  cType = value ? ALWAYS_TRUE : mGCCFix;
  cConditions.clear();
  cSwitches.clear();
}

void SimplifiedCondition::simplify() {

  // Special case; only one subcondition, so move it up (remove brackets)
  if (cSwitches.size() == 0 && cConditions.size() == 1) {
    cType = cConditions[0].cType;
    cSwitches = cConditions[0].cSwitches;
    cConditions = cConditions[0].cConditions;
  }

  // Integrate same gate sub-conditions into this one (remove brackets)
  for (int i = 0; i < cConditions.size(); i++) {
    if (cConditions[i].cType == cType) {
      for (int j = 0; j < cConditions[i].cSwitches.size(); j++) {
        cSwitches.push_back(cConditions[i].cSwitches[j]);
      }
      removeCondition(i);
    }
  }

  // Do AND / OR conversion
  for (int i = 0; i < cConditions.size(); i++) {
    switch (cType) {
      case AND:
      case NAND:
        // TODO:
        break;

      case OR:
      case NOR:
        // TODO
        break;
       
    }
  }

  // Handle constants and conflicts
  if (cType == AND || cType == NAND || cType == OR || cType == NOR) {
    for (int i = 0; i < cConditions.size(); i++) {
      if (cConditions[i].cType == NOT) {
        for (int j = 0; j < cSwitches.size(); j++) {
          if (cConditions[i].cSwitches[0] == cSwitches[j]) {
            setConstant(cType == NAND || cType == OR);
            return;
          }
        }
      } else if ((cConditions[i].cType == ALWAYS_TRUE) == (cType == AND || cType == NAND) || (cConditions[i].cType == ALWAYS_FALSE) == (cType == OR || cType == NOR)) {
        removeCondition(i);
      } else if ((cConditions[i].cType == ALWAYS_TRUE) == (cType == OR || cType == NOR) || (cConditions[i].cType == ALWAYS_FALSE) == (cType == AND || cType == NAND)) {
        setConstant(cType == NAND || cType == OR);
        return;
      }
    }
  }
}

void SimplifiedCondition::debug() {
  if (cType == ALWAYS_FALSE) {
    cout << "false";
    return;
  }
  if (cType == ALWAYS_TRUE) {
    cout << "true";
    return;
  }
  if (cType == NOT || cType == NAND || cType == NOR) {
    cout << "!";
  }
  if (cSwitches.size() + cConditions.size() > 1 || cType != NOT) {
    cout << "(";
  }
  for (int i = 0; i < cSwitches.size(); i++) {
    cout << (char) (cSwitches[i]);
    if (i < cSwitches.size() - 1) {
      if (cType == AND || cType == NAND) {
        cout << " & ";
      } else if (cType == OR || cType == NOR) {
        cout << " | ";
      }
    }
  }
  if (cSwitches.size() >= 1 && cConditions.size() >= 1) {
    if (cType == AND || cType == NAND) {
      cout << " & ";
    } else if (cType == OR || cType == NOR) {
      cout << " | ";
    }
  }
  for (int i = 0; i < cConditions.size(); i++) {
    cConditions[i].debug();
  }
  if (cSwitches.size() + cConditions.size() > 1 || cType != NOT) {
    cout << ")";
  }
}

