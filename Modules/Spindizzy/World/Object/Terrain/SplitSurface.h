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
#include <GL/glew.h>

#include "IsoRealms.h"

#include "Modules/Spindizzy/ISurface.h"
#include "Modules/Spindizzy/World/Common/CollisionData.h"

namespace IsoRealms::Spindizzy {
  class TerrainType;
  
  class SplitSurface : public ISurface {
    public:

    SplitSurface(bool splitDirection, int x, int y, int z, int southWestCorner, int southEastCorner, int northWestCorner, int northEastCorner, const std::optional<Condition>& condition, TerrainType& terrainType, IWorldObject& owner);
    SplitSurface(std::ifstream& cache, std::vector<ConditionElement*> elements, TerrainType& terrainType, IWorldObject& owner);
    std::optional<Condition>& getCondition();
    bool isAlternativeSplit() const;
    int getX() const;
    int getY() const;
    int getZ() const;
    int getHeightSW() const;
    int getHeightSE() const;
    int getHeightNW() const;
    int getHeightNE() const;
    
    /***********************\
     * Implements ISurface *
    \***********************/
    bool contains(LiteralVertex&, float) override;
    std::unique_ptr<CollisionData> getCollision(LiteralVertex&, LiteralVertex&, float, double startTime, double endTime) override;
    std::unique_ptr<CollisionData> getRollingEvent(LiteralVertex&, LiteralVertex&, double startTime, double endTime) override;
    float getHeightAt(float x, float y) const override;
    float getXAcceleration(float, float) override;
    float getYAcceleration(float, float) override;
    void notifyContact() override;
    void notifyImpact() override;
    float getSurfaceFriction() override;
    float getSurfaceGrip() override;
    float getSurfaceBounce() override;
    Respawn isRespawnAllowed() override;
    bool isRespawnPossible() override;
    void getRestingLocation(LiteralVertex&) override;
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

    static void render(float x, float y, float z, const AnimatedFloat heights[2][2], bool split);
    
    private:

    TerrainType& cType;
    IWorldObject& cOwner;
    
    int cDefX;
    int cDefY;
    int cDefZ;
    int cDefCornerHeights[2][2];
    bool cDefAlternativeSplit;
    std::optional<Condition> cDefCondition;
    
    std::unique_ptr<LiteralVertex> getSplitCrossingPoint(LiteralVertex&, LiteralVertex&, float*);
    std::unique_ptr<LiteralVertex> getBoundaryCrossingPoint(LiteralVertex&, LiteralVertex&, float*, float);
    bool hasFlatSide();
    float getHeightAt(float, float, bool) const;
    void confine(double*, double*);
    bool inNorthSplit(float, float) const;
    std::unique_ptr<CollisionData> getImpactCollision(LiteralVertex&, LiteralVertex&, bool);
  };
}
