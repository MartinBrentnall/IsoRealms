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
#ifndef SWITCH_LOGIC_H
#define SWITCH_LOGIC_H

/**
 * The SwitchSystem is an abstract class representing a system of activating
 * and deactivating arbitrary switches.
 */
class SwitchLogic {
  public:

  /**
   * Activate the specified switch.
   *
   * @param int  The switch to activate.
   * @returns    true if the switch states changed, otherwise false.
   */
  virtual bool activate(int) = 0;

  /**
   * Deactivate the specified switch.
   *
   * @param int  The switch to deactivate.
   * @returns    true if the switch states change, otherwise false.
   */
  virtual bool deactivate(int) = 0;

  /**
   * Test whether the specified switch is active.
   *
   * @param int  The switch to test.
   * @return     true if the switch is active, otherwise false.
   */
  virtual bool isActive(int) = 0;
};

#endif
