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
  template <typename TYPE> struct ResourceContainerTraits;

  class Spindizzy : public IModuleHandle {
    public:
    
    // Module constants.
    static const double DEFAULT_VIEW_ANGLE_PITCH;
    static const double DEFAULT_VIEW_ANGLE_YAW;
    
    // Module constructor.
    Spindizzy(Project& project, IResourceTypeRegistry& registry);

    /****************************\
     * Implements IModuleHandle *
    \****************************/
    void registerAssets(ResourceAssetRegistry& assets) override;
    void updateInputs(unsigned int milliseconds) override;
    void updateRuntime(unsigned int milliseconds) override;
    void updateEditing(unsigned int milliseconds) override;
    void reset() override;

    private:

    // Resource type definitions.
    ResourceTypeDefinition<Spindizzy, Ball>               cResourceBall;
    ResourceTypeDefinition<Spindizzy, C64LiftGraphics>    cResourceC64LiftGraphics;
    ResourceTypeDefinition<Spindizzy, C64TerrainGraphics> cResourceC64TerrainGraphics;
    ResourceTypeDefinition<Spindizzy, DebrisChunk>        cResourceDebrisChunk;
    ResourceTypeDefinition<Spindizzy, Gyroscope>          cResourceGyroscope;
    ResourceTypeDefinition<Spindizzy, Jewel>              cResourceJewel;
    ResourceTypeDefinition<Spindizzy, Top>                cResourceTop;

    // Scripting support.
    template <class TYPE> friend struct AssetContainerTraits;
    template <class TYPE> friend struct ResourceContainerTraits;
  };

  template<> struct ResourceContainerTraits<Ball>               {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceBall;              }};
  template<> struct ResourceContainerTraits<C64LiftGraphics>    {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceC64LiftGraphics;   }};
  template<> struct ResourceContainerTraits<C64TerrainGraphics> {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceC64TerrainGraphics;}};
  template<> struct ResourceContainerTraits<DebrisChunk>        {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceDebrisChunk;       }};
  template<> struct ResourceContainerTraits<Gyroscope>          {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceGyroscope;         }};
  template<> struct ResourceContainerTraits<Jewel>              {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceJewel;             }};
  template<> struct ResourceContainerTraits<Top>                {template <typename SPINDIZZY> static auto& get(SPINDIZZY& spindizzy) {return spindizzy.cResourceTop;               }};
}
