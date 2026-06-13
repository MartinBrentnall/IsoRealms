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

#include "Ball/Ball.h"
#include "C64LiftGraphics/C64LiftGraphics.h"
#include "C64TerrainGraphics/C64TerrainGraphics.h"
#include "DebrisChunk/DebrisChunk.h"
#include "Gyroscope/Gyroscope.h"
#include "Jewel/Jewel.h"
#include "Top/Top.h"

namespace IsoRealms::Spindizzy {
  template <typename TYPE> struct AssetContainerTraits;
  template <typename TYPE> struct ComponentContainerTraits;

  class Spindizzy : public IModuleHandle {
    public:
    
    // Module constants.
    inline static const double DEFAULT_VIEW_ANGLE_PITCH = -90.0 + std::atan(1.0 / std::sqrt(2.0)) * (180.0 / M_PI);
    inline static const double DEFAULT_VIEW_ANGLE_YAW   = -45.0;
    
    // Module constructor.
    Spindizzy(Project& project, IComponentTypeRegistry& registry);

    /****************************\
     * Implements IModuleHandle *
    \****************************/
    void registerAssets(ComponentAssetRegistry& assets) override;
    void updateInputs(unsigned int milliseconds) override;
    void updateRuntime(unsigned int milliseconds) override;
    void updateEditing(unsigned int milliseconds) override;
    void reset() override;

    private:

    // Component type definitions.
    ComponentTypeDefinition<Spindizzy, Ball>               cComponentBall;
    ComponentTypeDefinition<Spindizzy, C64LiftGraphics>    cComponentC64LiftGraphics;
    ComponentTypeDefinition<Spindizzy, C64TerrainGraphics> cComponentC64TerrainGraphics;
    ComponentTypeDefinition<Spindizzy, DebrisChunk>        cComponentDebrisChunk;
    ComponentTypeDefinition<Spindizzy, Gyroscope>          cComponentGyroscope;
    ComponentTypeDefinition<Spindizzy, Jewel>              cComponentJewel;
    ComponentTypeDefinition<Spindizzy, Top>                cComponentTop;

    // Scripting support.
    template <class TYPE> friend struct AssetContainerTraits;
    template <class TYPE> friend struct ComponentContainerTraits;
  };

  template<> struct ComponentContainerTraits<Ball>               {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cComponentBall;              }};
  template<> struct ComponentContainerTraits<C64LiftGraphics>    {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cComponentC64LiftGraphics;   }};
  template<> struct ComponentContainerTraits<C64TerrainGraphics> {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cComponentC64TerrainGraphics;}};
  template<> struct ComponentContainerTraits<DebrisChunk>        {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cComponentDebrisChunk;       }};
  template<> struct ComponentContainerTraits<Gyroscope>          {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cComponentGyroscope;         }};
  template<> struct ComponentContainerTraits<Jewel>              {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cComponentJewel;             }};
  template<> struct ComponentContainerTraits<Top>                {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cComponentTop;               }};
}
