/*
 * Copyright 2023 Martin Brentnall
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
#pragma once

#include <algorithm>
#include <cmath>
#include <optional>

#include "IsoRealms/Common/IVisualElement.h"
#include "IsoRealms/IAssetBrowser.h"
#include "IsoRealms/Literals.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/ISurface.h"
#include "Modules/Spindizzy/IWorldObject.h"
#include "Modules/Spindizzy/World/Common/CollisionData.h"
#include "Modules/Spindizzy/World/Common/IPhysicalObject.h"

namespace IsoRealms::Spindizzy {
  class InfiniteSurface : public ISurface {
    public:
    InfiniteSurface(Zone& zone, float height);

    /***********************\
     * Implements ISurface *
    \***********************/
    std::unique_ptr<CollisionData> getCollision(LiteralVertex&, LiteralVertex&, float, double startTime, double endTime) override;
    std::unique_ptr<CollisionData> getRollingEvent(LiteralVertex&, LiteralVertex&, double startTime, double endTime) override;
    float getHeightAt(float x, float y) const override;
    bool contains(LiteralVertex&, float) override;
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
    void saveCache(std::ostream& cache, bool physical) override;
    int getXStart() const override;
    int getXEnd() const override;
    int getYStart() const override;
    int getYEnd() const override;
    bool isSolid() override;
    void adjustPosition(LiteralVertex& location, double milliseconds) override;
    IWorldObject* getOwner() override;
    Zone& getZone() override;
    
    void render() const override;
    void renderOutline() const override;
    void renderHighlight() const override;
    int getSurfaceCellHeight(int x, int y) const override; 
    int getSurfaceCellElevation(int x, int y) const override;
    bool alligned(int x, int y) const override;
    std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals() override;    
    
    private:
    
    // External interfaces.
    Zone& cZone;
    
    // TODO: Is this definition data????
    const float cDefHeight;

    std::unique_ptr<LiteralVertex> getBoundaryCrossingPoint(LiteralVertex& start, LiteralVertex& end, double* lowestGradient, double infinity);
  };
}
