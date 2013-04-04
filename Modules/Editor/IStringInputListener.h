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
#ifndef I_STRING_INPUT_LISTENER_H
#define I_STRING_INPUT_LISTENER_H

#include <string>

class IStringInputListener {
  public:
  virtual ~IStringInputListener() {}

  /**
   * Called to notify the listener that a string has been entered.
   * 
   * @param string  The string that has been entered by the user.
   */
  virtual void stringEntered(string) = 0;
};

#endif
