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
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "IRectangle.h"

class Rectangle:public IRectangle {
  private:
  float cTop;
  float cLeft;
  float cBottom;
  float cRight;

  public:
  Rectangle(float, float, float, float);

  bool contains(Rectangle&);

  /*************************\
   * Implements IRectangle *
  \*************************/
  float getTop();
  float getLeft();
  float getBottom();
  float getRight();
};

#endif
