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

#include "Modules/Spindizzy/Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitBoundary.h"
#include "Modules/Spindizzy/Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitCellLocation.h"
#include "Modules/Spindizzy/Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitChaser.h"
#include "Modules/Spindizzy/Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitModel.h"
#include "Modules/Spindizzy/Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitMovable.h"
#include "Modules/Spindizzy/Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitPhysics.h"
#include "Modules/Spindizzy/Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitSpinner.h"
#include "Modules/Spindizzy/Assets/Type/IZoneObjectTypeTrait.h"
#include "Modules/Spindizzy/ZoneObjectType/ZoneObjectType.h"

namespace IsoRealms::Spindizzy {
  class ZoneObjectTypeTraitRegistry : public AssetClientManager<ZoneObjectTypeTraitRegistry, ZoneObjectType, IZoneObjectTypeTrait> {
    public:
    ZoneObjectTypeTraitRegistry(IResourceTypeRegistry& registry);

    private:
    AssetInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitBoundary>     cBoundary;
    AssetInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitCellLocation> cCellLocation;
    AssetInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitChaser>       cChaser;
    AssetInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitModel>        cModel;
    AssetInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitMovable>      cMovable;
    AssetInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitPhysics>      cPhysics;
    AssetInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitSpinner>      cSpinner;

  };
}
