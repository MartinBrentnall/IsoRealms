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

#include "Modules/Equilibria/Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitBoundary.h"
#include "Modules/Equilibria/Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitCellLocation.h"
#include "Modules/Equilibria/Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitChaser.h"
#include "Modules/Equilibria/Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitModel.h"
#include "Modules/Equilibria/Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitMovable.h"
#include "Modules/Equilibria/Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitPhysics.h"
#include "Modules/Equilibria/Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitSpinner.h"
#include "Modules/Equilibria/Resources/Type/IZoneObjectTypeTrait.h"
#include "Modules/Equilibria/ZoneObjectType/ZoneObjectType.h"

namespace IsoRealms::Equilibria {
  class ZoneObjectTypeTraitRegistry : public ResourceClientManager<ZoneObjectTypeTraitRegistry, ZoneObjectType, IZoneObjectTypeTrait> {
    public:
    ZoneObjectTypeTraitRegistry(IComponentTypeRegistry& registry);

    private:
    ResourceInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitBoundary>     cBoundary;
    ResourceInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitCellLocation> cCellLocation;
    ResourceInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitChaser>       cChaser;
    ResourceInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitModel>        cModel;
    ResourceInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitMovable>      cMovable;
    ResourceInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitPhysics>      cPhysics;
    ResourceInstanced<ZoneObjectType, IZoneObjectTypeTrait, ZoneObjectTypeTraitSpinner>      cSpinner;
  };
}
