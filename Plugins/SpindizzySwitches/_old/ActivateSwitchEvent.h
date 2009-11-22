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
#ifndef ACTIVATE_SWITCH_EVENT_H
#define ACTIVATE_SWITCH_EVENT_H

#include "Configuration.h"
#include "SwitchLogic.h"
#include "Event.h"

#include <iostream>

class ActivateSwitchEvent:public Event {
  private:
  static SwitchLogic* cSwitchLogic;
  int cSwitchToActivate;

  public:
  static void init();

  /**
   * Construct the event specified the switch to activate.
   */
  ActivateSwitchEvent(int);

  /**************************************************************************\
   * Implemented method of Event.h                                  *
  \**************************************************************************/
  void execute();
};

#endif
