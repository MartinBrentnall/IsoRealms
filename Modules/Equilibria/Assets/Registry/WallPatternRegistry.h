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

#include <functional>

#include "IsoRealms.h"

#include "Modules/Equilibria/Assets/Fixed/WallPattern/WallPatternCap.h"
#include "Modules/Equilibria/Assets/Fixed/WallPattern/WallPatternComposite.h"
#include "Modules/Equilibria/Assets/Fixed/WallPattern/WallPatternOutline.h"
#include "Modules/Equilibria/Assets/Fixed/WallPattern/WallPatternTile.h"
#include "Modules/Equilibria/Assets/Type/IWallPattern.h"
#include "Modules/Equilibria/TerrainType/TerrainType.h"

namespace IsoRealms::Equilibria {
  class WallPatternRegistry : public AssetClientManager<WallPatternRegistry, TerrainType, IWallPattern> {
    public:
    WallPatternRegistry(IResourceTypeRegistry& registry);

    private:
    AssetInstanced<TerrainType, IWallPattern, WallPatternCap>       cCapped;
    AssetInstanced<TerrainType, IWallPattern, WallPatternComposite> cComposite;
    AssetInstanced<TerrainType, IWallPattern, WallPatternOutline>   cOutline;
    AssetInstanced<TerrainType, IWallPattern, WallPatternTile>      cTile;
  };
}
