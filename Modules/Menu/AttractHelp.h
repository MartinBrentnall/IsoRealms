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
#ifndef ATTRACT_HELP_H
#define ATTRACT_HELP_H

#include <iostream>
#include <vector>
#include <SDL/SDL.h>

#include "IAttract.h"

/**
 * This attract scene displays some helpful information for a limited time.
 */
class AttractHelp:public IAttract {
  public:

  /**
   * Default constructor.
   */
  AttractHelp();

  /**************************************************************************\
   * Implemented methods of IAttract.h                                      *
  \**************************************************************************/
  void init();
  void update(int);
  void render();
  bool hasFinished();
  void frontEndActive(bool);
};

#endif
