/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef I_RECTANGLE_H
#define I_RECTANGLE_H

class IRectangle {
  public:

  /**
   * Get the location of the left edge of the rectangle.
   */
  virtual float getLeft() = 0;

  /**
   * Get the location of the right edge of the rectangle.
   */
  virtual float getRight() = 0;

  /**
   * Get the location of the top edge of the rectangle.
   */
  virtual float getTop() = 0;

  /**
   * Get the location of the bottom edge of the rectangle.
   */
  virtual float getBottom() = 0;
};

#endif
