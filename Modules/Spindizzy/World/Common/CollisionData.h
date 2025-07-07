/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "IsoRealms.h"

#include "Modules/Spindizzy/ISurface.h"

namespace IsoRealms::Spindizzy {
  class Wall;

  class CollisionData final {
    public:
    enum class Type {
      SURFACE_LEAVE,
      SURFACE_MOUNT,
      SURFACE_MOVEMENT,
      WALL_IMPACT,
      WALL_CLIP,
      WALL_LEAVE
    };
    
    CollisionData(ISurface* surface, Type type, LiteralVertex location, float xSlope, float ySlope, float gradient);
    // TODO: Probably should be split to separate class for wall collisions
    CollisionData(Wall* wall, Type type, LiteralVertex location, float gradient);
      
    ISurface* getSurface();
    Wall* getWall();
    Type getType();
    LiteralVertex getEventLocation();
    float getXSlope();
    float getYSlope();
    float getGradient();
    
    private:
    Type cType;
    
    struct Surface {
      ISurface* cSurface;
      float cXSlope;
      float cYSlope;
      
      Surface(ISurface* surface, float xSlope, float ySlope) :
                cSurface(surface),
                cXSlope(xSlope),
                cYSlope(ySlope) {
      }
    };
    
    union Thing {
      Surface cSurface;
      Wall* cWall;
      
      Thing(ISurface* surface, float xSlope, float ySlope) :
                cSurface(surface, xSlope, ySlope) {
      }
      
      Thing(Wall* wall) :
                cWall(wall) {
      }
    } cThing;
    LiteralVertex cLocation;
    float cGradient;
  };
}
