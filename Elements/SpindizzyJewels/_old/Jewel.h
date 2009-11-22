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
#ifndef JEWEL_H
#define JEWEL_H

#include <math.h>

#include "General3DDrawing.h"
#include "SpindizzyConstants.h"
#include "Coordinate.h"
#include "Point.h"
#include "Colour.h"

class Jewel:public SpindizzyConstants, General3DDrawing {
  private:

  /** Display list id used for drawing the yellow frame of the jewel. */
  static int cFrameDisplayList;

  /** Display list id used for drawing the coloured panels of the jewel. */
  static int cPanelDisplayList;

  /** true if the jewel has been collected by the player, otherwise false. */
  bool cCollected;

  public:
  
  /**
   * Initialise display lists used for rendering jewels.
   */
  static void initDisplayLists();

  /**
   * Construct a jewel to be placed at 0,0,0.
   */ 
  Jewel();

  /**
   * Construct a jewel to be placed at the specified location.
   *
   * @param Point  Destination location of this jewel.
   */
  Jewel(Point location);

  /**
   * Set the location of this jewel.
   *
   * @param Point  Destination location of this jewel.
   */
  void setLocation(Point location);

  /**
   * Determines whether this jewel has been collected by the player.
   *
   * @returns  true if this jewel has been collected, otherwise false.
   */
  bool isCollected();

  /**
   * Determines whether this jewel is at the specified location.
   *
   * @returns  true if this jewel is at the specified location.
   */
  bool isCollected(Coordinate&, Coordinate&);

  /**
   * Render the jewel, updated by the specified amount of time.
   */
  void render(int ticks, float alpha);

  /**
   * Write the jewel data to the specified file.
   */
  void write(FILE *w);
};

#endif
