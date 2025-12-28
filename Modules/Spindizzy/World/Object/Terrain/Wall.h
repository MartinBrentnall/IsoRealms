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

#include <iostream>

#include "IsoRealms.h"

#include "Modules/Spindizzy/World/Common/CollisionData.h"

namespace IsoRealms::Spindizzy {
  class IPhysicalObject;
  class IWallPattern;
  class PhysicalState;
  class TerrainType;

  class Wall {
    public:
    enum class Direction {
      NORTH,
      SOUTH,
      EAST,
      WEST
    };

    class WallEdge {
      private:
      int cStartHeight;
      int cEndHeight;

      public:
      WallEdge(int, int);

      int getStartHeight();
      int getEndHeight();
    };

    Wall(int x, int y, int z, int length, int height, int topSlope, int bottomSlope, Direction direction, std::optional<Condition>& condition, TerrainType& type, IWorldObject& object);
    const std::optional<Condition>& getCondition();

    float getHeightAt(float);
    float getLowAt(float);

    int getX();
    int getY();
    int getZ();
    int getLength();
    int getHeight();
    int getTopSlope();
    int getBottomSlope();
    Direction getFaceDirection();
    bool isAtZoneEdge();
    void updateState(PhysicalState& state);


    std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals();
    void render();
    void renderOutline();
    void renderSelectionHighlight();


    WallEdge getTopEdge(int);
    WallEdge getBottomEdge(int);
    void saveCache(std::ostream& cache, bool physical);

    int getXStart() const;
    int getXEnd() const;
    int getYStart() const;
    int getYEnd() const;
    int getZStart() const;
    int getZEnd() const;

    std::unique_ptr<CollisionData> getCollision(LiteralVertex& start, LiteralVertex& end, IPhysicalObject* object);
    float getBounce();
    std::unique_ptr<CollisionData> getSlidingEvent(LiteralVertex& start, LiteralVertex& end, IPhysicalObject* object);
    IWorldObject* getOwner();
    Zone& getZone();

    void bindValues();
    void unbindValues();
    
    private:

    // External interfaces.
    IWorldObject& cOwner; /// Object to which this wall belongs.
    TerrainType& cType;   /// Terrain type to which this wall belongs.
    
    // Definition data.
    const int cDefX;                                  /// Starting X location of the wall surface.
    const int cDefY;                                  /// Starting Y location of the wall surface.
    const int cDefZ;                                  /// Starting Z location (bottom) of the wall surface.
    const int cDefLength;                             /// Length of the wall surface.
    const int cDefHeight;                             /// Height of the wall surface from the bottom.
    const int cDefTopSlope;                           /// Slopiness at the top.
    const int cDefBottomSlope;                        /// Slopiness at the bottom.
    const Direction cDefFacing;                       /// The face direction of the wall surface.
    const std::optional<Condition> cDefCondition;     /// Condition under which the wall is present.

    float getSouthEdge(float);
    float getEastEdge(float);
    float getNorthEdge(float);
    float getWestEdge(float);
    bool contains(LiteralVertex&, float, float, float);
    std::unique_ptr<CollisionData> checkVerticalEdgeCollision(LiteralVertex&, LiteralVertex&, float, float, float, bool);
  };
}
