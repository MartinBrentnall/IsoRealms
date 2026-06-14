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

#include "Modules/Equilibria/Resources/Fixed/SurfacePattern/SurfacePatternComposite.h"
#include "Modules/Equilibria/Resources/Fixed/SurfacePattern/SurfacePatternOutline.h"
#include "Modules/Equilibria/Resources/Fixed/SurfacePattern/SurfacePatternTile.h"
#include "Modules/Equilibria/Resources/Fixed/SurfacePattern/SurfacePatternSplitVariant.h"
#include "Modules/Equilibria/Resources/Type/ISurfacePattern.h"
#include "Modules/Equilibria/TerrainType/TerrainType.h"

namespace IsoRealms::Equilibria {
  class SurfacePatternRegistry : public ResourceClientManager<SurfacePatternRegistry, TerrainType, ISurfacePattern> {
    public:
    SurfacePatternRegistry(IComponentTypeRegistry& registry);

    private:
    ResourceInstanced<TerrainType, ISurfacePattern, SurfacePatternComposite>    cComposite;
    ResourceInstanced<TerrainType, ISurfacePattern, SurfacePatternOutline>      cOutline;
    ResourceInstanced<TerrainType, ISurfacePattern, SurfacePatternSplitVariant> cSplitVariant;
    ResourceInstanced<TerrainType, ISurfacePattern, SurfacePatternTile>         cTile;
  };
}
