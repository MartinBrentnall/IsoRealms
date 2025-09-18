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

#include "Modules/Spindizzy/Assets/Fixed/ZoneViewType/ZoneViewTypeActual.h"
#include "Modules/Spindizzy/Assets/Fixed/ZoneViewType/ZoneViewTypeOverview.h"
#include "Modules/Spindizzy/Assets/Type/IZoneViewType.h"
#include "Modules/Spindizzy/WorldView/WorldView.h"

namespace IsoRealms::Spindizzy {
  class ZoneViewTypeRegistry : public AssetClientManager<ZoneViewTypeRegistry, WorldView, IZoneViewType> {
    public:
    ZoneViewTypeRegistry(IResourceTypeRegistry& registry);

    private:
    AssetInstanced<WorldView, IZoneViewType, ZoneViewTypeActual>   cActual;
    AssetInstanced<WorldView, IZoneViewType, ZoneViewTypeOverview> cOverview;
  };
}
