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

#include <cmath>
#include <cstdlib>
#include <GL/glew.h>

#include "IsoRealms.h"

#include "Modules/Spindizzy/ISurface.h"
#include "Modules/Spindizzy/World/Common/CollisionData.h"

namespace IsoRealms::Spindizzy {
  class TerrainType;
  
  class Surface : public ISurface {
    public:

    Surface(int north, int east, int south, int west, int height, int westEastSlope, int northSouthSlope, Direction facing, const std::optional<Condition>& condition, TerrainType* terrainType, IWorldObject& owner);
    Surface(std::ifstream& cache, std::vector<ConditionElement*> elements, TerrainType& terrainType, IWorldObject& owner);
    
    // 
    std::optional<Condition>& getCondition();
    int getZ() const;
    int getXSlope() const;
    int getYSlope() const;
    Direction getFacing() const;

    /***********************\
     * Implements ISurface *
    \***********************/
    bool contains(LiteralVertex&, float) override;
    std::unique_ptr<CollisionData> getCollision(LiteralVertex& start, LiteralVertex& end, float stepHeight, double startTime, double endTime) override;
    std::unique_ptr<CollisionData> getRollingEvent(LiteralVertex& start, LiteralVertex& end, double startTime, double endTime) override;
    float getHeightAt(float x, float y) const override;
    float getXAcceleration(float x, float y) override;
    float getYAcceleration(float x, float y) override;
    void notifyContact() override;
    void notifyImpact() override;
    float getSurfaceFriction() override;
    float getSurfaceGrip() override;
    float getSurfaceBounce() override;
    Respawn isRespawnAllowed() override;
    bool isRespawnPossible() override;
    void getRestingLocation(LiteralVertex& location) override;
    int getXStart() const override;
    int getXEnd() const override;
    int getYStart() const override;
    int getYEnd() const override;
    IWorldObject* getOwner() override;
    Zone& getZone() override;
    bool isSolid() override;
    void adjustPosition(LiteralVertex& location, double milliseconds) override;
    int getSurfaceCellHeight(int x, int y) const override;
    int getSurfaceCellElevation(int x, int y) const override;
    bool alligned(int x, int y) const override;
    void saveCache(std::ostream& cache, bool physical) override;
    void render() const override;
    void renderOutline() const override;
    void renderHighlight() const override;
    std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals() override;

    private:
    
    // External interfaces.
    IWorldObject& cOwner;
    TerrainType& cType;
    
    // Definition data.
    int cDefStartX;
    int cDefEndX;
    int cDefStartY;
    int cDefEndY;
    int cDefZ;
    int cDefSlopeX;
    int cDefSlopeY;
    Direction cDefFacing;
    std::optional<Condition> cDefCondition;

    std::unique_ptr<LiteralVertex> getBoundaryCrossingPoint(LiteralVertex& start, LiteralVertex& end, double* lowestGradient, double infinity);
  };
}
