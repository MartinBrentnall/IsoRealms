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
#include "SpindizzySwitchLogic.h"

SpindizzySwitchLogic::SpindizzySwitchLogic() {
  cActiveSwitchA = SWITCH_NONE;
  cActiveSwitchB = SWITCH_NONE;
  cNextSwitch = &cActiveSwitchA;
}

bool SpindizzySwitchLogic::activate(int symbol) {
  bool mStateChanged = false;
  switch (symbol) {
    case SWITCH_DIAMOND_NONE: // Reset switch
      if (cActiveSwitchA != SWITCH_NONE || cActiveSwitchB != SWITCH_NONE) {
        cActiveSwitchA = SWITCH_NONE;
        cActiveSwitchB = SWITCH_NONE;
        cNextSwitch = &cActiveSwitchA;
        mStateChanged = true;
      }
      break;

    case SWITCH_CIRCLE_NONE:
    case SWITCH_CIRCLE_LEFT:
    case SWITCH_CIRCLE_RIGHT:
    case SWITCH_CIRCLE_BOTH:
      if (cActiveSwitchA != symbol && cActiveSwitchB != symbol) {
        *cNextSwitch = symbol;
        cNextSwitch = (cNextSwitch == &cActiveSwitchA ? &cActiveSwitchB : &cActiveSwitchA);
        mStateChanged = true;
      }
      break;

    default:
      if (cActiveSwitchA != symbol || (cActiveSwitchB >= 0 && cActiveSwitchB <= 3)) {
        cActiveSwitchA = symbol;
        cActiveSwitchB = SWITCH_NONE;
        cNextSwitch = &cActiveSwitchB;
        mStateChanged = true;
      }
      break;
  }
  return mStateChanged;
}

bool SpindizzySwitchLogic::deactivate(int symbol) {
  // Nothing to do; Spindizzy doesn't use switch deactivation.
  return false;
}

bool SpindizzySwitchLogic::isActive(int symbol) {
  return symbol == SWITCH_NONE || symbol == cActiveSwitchA || symbol == cActiveSwitchB;
}
