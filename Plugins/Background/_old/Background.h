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
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Colour.h"
#include "MiscFunctions.h"
#include "SpindizzyConstants.h"

/**
 * The simple Background class implements a mechanism for changing background
 * colours by fading from one colour to another over a fixed period of time.
 */
class Background:public SpindizzyConstants {
  private:
  /** Current colour, or the colour fading to. */
  Colour cCurrentColour;

  /** Colour fading from. */
  Colour cPreviousColour;

  /** A value from 0 (previous colour) to 1 (current colour), determines how far the fade has progressed. */
  float cProgress;

  public:
  // TODO: Implement mixable-visual interface for camera and colour. (e.g.  Mixable.mixWith(Mixable mixable, float weight).render();)
  /**
   * Construct a black background.
   */
  Background();

  /**
   * Construct a background of the specified colour.
   *
   * @param Colour&  Colour of the background.
   */
  Background(Colour& colour);
  
  /**
   * Set this background to the specified colour.  The background will fade
   * from the current colour to the specified colour over a set period of time.
   *
   * @param Colour&  Colour to fade the background to.
   */
  void set(Colour& colour);

  /**
   * Update the background colour by the specified number of ticks, and set the
   * resulting colour to the background colour (glClearColor).  This is used
   * when fading from one background colour to another via calls to set().
   *
   * @param int  Time passed since previous frame.
   */
  void update(int ticks);
};

#endif
