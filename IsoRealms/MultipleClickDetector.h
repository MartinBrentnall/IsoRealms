/*
 * Copyright 2015 Martin Brentnall
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
#ifndef MULTIPLE_CLICK_DETECTOR_H
#define MULTIPLE_CLICK_DETECTOR_H

#include <iostream>
#include <SDL/SDL.h>

class MultipleClickDetector {
  private:
  static const unsigned int CLICK_INTERVAL_THRESHOLD;

  unsigned int cClicks;
  Uint8 cButton;
 
  unsigned int cTimeSinceLastClick;

  public:
  static const unsigned int SINGLE_CLICK;
  static const unsigned int DOUBLE_CLICK;
  static const unsigned int TRIPLE_CLICK;
    
  MultipleClickDetector();
  void input(SDL_Event&);
  void update(unsigned int);
  unsigned int getClicks();
};

#endif
