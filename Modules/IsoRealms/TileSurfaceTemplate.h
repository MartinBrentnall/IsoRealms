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
#ifndef TILE_SURFACE_TEMPLATE_H
#define TILE_SURFACE_TEMPLATE_H

#include <IsoRealms/Resources/GeometryProcessor/ITileSurfaceTemplate.h>

class TileSurfaceTemplate:public ITileSurfaceTemplate {
  private:
  int cNorth;
  int cSouth;
  int cEast;
  int cWest;
  Condition* cCondition;

  public:
  TileSurfaceTemplate(int, int, int, int, Condition* = NULL);

  int getNorth();
  int getEast();
  int getSouth();
  int getWest();
  Condition* getCondition();
  bool alligned(int, int);
};

#endif
