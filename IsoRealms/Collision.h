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
#ifndef COLLISION_H
#define COLLISION_H

#include <cmath>

#include "BlockArea.h"
#include "CollisionVertex.h"
#include "Resources/Vertex/Vertex.h"

class Collision {
  public:
  static void clamp(float, float, float*);
  static float getCrossingPoint(float, float, float);
  static float getCrossingPoint(float, float, float, float);
  static bool contains(Vertex&, float, float, float, float, float, float);
  static CollisionVertex* getXCrossingPoint(Vertex&, Vertex&, float); 
  static CollisionVertex* getYCrossingPoint(Vertex&, Vertex&, float);
  static CollisionVertex* getZCrossingPoint(Vertex&, Vertex&, float);
/*  static CollisionVertex* getCrossingPoint(float, float, float, float, float, float, float);
  static CollisionVertex* getCrossingPoint(float, float, float, float, float, float, float, float);
  static CollisionVertex* getXCrossingPoint(Vertex&, Vertex&, float, float, float, float);
  static CollisionVertex* getYCrossingPoint(Vertex&, Vertex&, float, float, float, float);
  static CollisionVertex* getZCrossingPoint(Vertex&, Vertex&, float, float, float, float);*/
  static CollisionVertex* getXCrossingPoint(Vertex&, Vertex&, float, float, float, float, float);
  static CollisionVertex* getYCrossingPoint(Vertex&, Vertex&, float, float, float, float, float);
  static CollisionVertex* getZCrossingPoint(Vertex&, Vertex&, float, float, float, float, float);
  static CollisionVertex* getXCrossingPoint(Vertex&, Vertex&, float, BlockArea&);
  static CollisionVertex* getYCrossingPoint(Vertex&, Vertex&, float, BlockArea&);
  static CollisionVertex* getZCrossingPoint(Vertex&, Vertex&, float, BlockArea&);
  static CollisionVertex* getCrossingPoint(Vertex&, Vertex&, float, float, float, float, float, float, bool);
  static bool hasCrossed(Vertex&, Vertex&, float, float, float, float, float, float, bool);
  static CollisionVertex* getEntryPoint(Vertex&, Vertex&, float, float, float, float, float, float);
  static CollisionVertex* getExitPoint(Vertex&, Vertex&, float, float, float, float, float, float);
  static float getCrossingPoint(Vertex&, Vertex&, Vertex&, float, bool);
  static float getEntryPoint(Vertex&, Vertex&, Vertex&, float);
  static float getExitPoint(Vertex&, Vertex&, Vertex&, float);
  static float getCrossingPoint(Vertex&, Vertex&, float, Vertex&, Vertex&, float, bool);
  static float getEntryPoint(Vertex&, Vertex&, float, Vertex&, Vertex&, float);
  static float getExitPoint(Vertex&, Vertex&, float, Vertex&, Vertex&, float);
};

#endif