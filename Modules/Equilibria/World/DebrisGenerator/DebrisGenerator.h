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

#include <deque>

#include "IsoRealms.h"

#include "Modules/Equilibria/World/Object/Zone/Zone.h"

#include "Debris.h"

namespace IsoRealms::Equilibria {
  class DebrisGenerator final {
    public:
    DebrisGenerator(JSONObject object, IResourceData& owner);
    void registerAssets(ResourceAssetRegistry& assets, const std::string& parentID);
    void updateRuntime(unsigned int milliseconds);
    void renderRuntime();
    void save(JSONObject object) const;
    void reset();
    
    float getStepReach() const;
    float getHeight() const;
    float getRadius() const;

    /*********************\
     * Scripting Support *
    \*********************/
    void generateDebris(IVertex* location, double xMomentum, double yMomentum, double zMomentum, Zone& zone);
    void clear();
    
    private:
    
    // JSON members.
    inline static const std::string JSON_APPEARANCE = "appearance";
    inline static const std::string JSON_HEIGHT     = "height";
    inline static const std::string JSON_ID         = "id";
    inline static const std::string JSON_LIFE       = "life";
    inline static const std::string JSON_RADIUS     = "radius";
    inline static const std::string JSON_STEP_REACH = "stepReach";

    inline static const float        DEFAULT_HEIGHT      = 0.7f;
    inline static const unsigned int DEFAULT_LIFE        = 2000U;
    inline static const float        DEFAULT_RADIUS      = 0.3f;
    inline static const float        DEFAULT_STEP_REACH  = 0.2f;

    // Definition data.
    std::string cDefID;        /// ID of this generator.
    Model cDefModel;           /// Visual representation of debris from this generator.
    unsigned int cDefLifeTime; /// Lifetime of debris from this generator.
    float cDefHeight;
    float cDefRadius;
    float cDefStepReach;
    
    // Runtime data
    std::deque<std::unique_ptr<Debris>> cRuntimeDebris; /// Generated debris with life remaining.
      
    LuaBinding<DebrisGenerator> cLuaBinding;
  };
}
