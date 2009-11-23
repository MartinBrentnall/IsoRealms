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
#ifndef SPINDIZZY_SWITCH_STATES_H
#define SPINDIZZY_SWITCH_STATES_H

#include "../../SwitchLogic.h"

#include <iostream>

/**
 * Implements switch logic from 8-bit Spindizzy game.  The logic consists of
 * seven primary switches, four secondary switches and a single reset switch.
 * <br>
 * Activating a primary switch causes all other switches to deactivate.
 * Activating a secondary switch causes the least recently activated switch to
 * deactivate if there are already two switches active.
 * Activating the reset switch causes all switches to deactivate.  Note that
 * reset switch does not have a state of its own.
 * <br>
 * Deactivate is not required or implemented.
 */
class SpindizzySwitchLogic:public SwitchLogic {
  private:

  // Four secondary switches
  static const int SWITCH_CIRCLE_BOTH = 0;
  static const int SWITCH_CIRCLE_LEFT = 1;
  static const int SWITCH_CIRCLE_RIGHT = 2;
  static const int SWITCH_CIRCLE_NONE = 3;

  // Seven primary switches
  static const int SWITCH_SQUARE_BOTH = 4;
  static const int SWITCH_SQUARE_LEFT = 5;
  static const int SWITCH_SQUARE_RIGHT = 6;
  static const int SWITCH_SQUARE_NONE = 7;
  static const int SWITCH_DIAMOND_BOTH = 8;
  static const int SWITCH_DIAMOND_LEFT = 9;
  static const int SWITCH_DIAMOND_RIGHT = 10;

  // One reset switch
  static const int SWITCH_DIAMOND_NONE = 11;
  static const int SWITCH_NONE = SWITCH_DIAMOND_NONE;

  /** First active switch. */
  int cActiveSwitchA;

  /** Second active switch. */
  int cActiveSwitchB;

  /** Points to the next switch to deactivate for circular switches (least recently activated). */
  int *cNextSwitch; 

  public:

  /**
   * Constructs the switch logic object with no active switches.
   */
  SpindizzySwitchLogic();

  /**************************************************************************\
   * Implemented methods of SwitchLogic.h                                   *
  \**************************************************************************/
  bool activate(int);
  bool deactivate(int);
  bool isActive(int);
};

#endif
