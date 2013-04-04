/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#ifndef I_MENU_STACK_H
#define I_MENU_STACK_H

#include "IApplicableItem.h"

class FrontEndMenu;

class IMenuStack {
  public:
  virtual void push(FrontEndMenu*) = 0;
  virtual void pop() = 0;
  virtual void testOnExit(FrontEndMenu*, IApplicableItem*) = 0;
};

#endif
