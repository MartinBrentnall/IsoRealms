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
#include "ZoneObjectTypeTraitRegistry.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  ZoneObjectTypeTraitRegistry::ZoneObjectTypeTraitRegistry(IResourceTypeRegistry& registry) :
            cBoundary(registry.getAssetMetadata("ZoneObjectTypeTraitBoundary")),
            cCellLocation(registry.getAssetMetadata("ZoneObjectTypeTraitCellLocation")),
            cChaser(registry.getAssetMetadata("ZoneObjectTypeTraitChaser")),
            cModel(registry.getAssetMetadata("ZoneObjectTypeTraitModel")),
            cMovable(registry.getAssetMetadata("ZoneObjectTypeTraitMovable")),
            cPhysics(registry.getAssetMetadata("ZoneObjectTypeTraitPhysics")),
            cSpinner(registry.getAssetMetadata("ZoneObjectTypeTraitSpinner")) {
    add(&cBoundary,     "Boundary",     "Spindizzy");
    add(&cCellLocation, "CellLocation", "Spindizzy");
    add(&cChaser,       "Chaser",       "Spindizzy");
    add(&cModel,        "Model",        "Spindizzy");
    add(&cMovable,      "Movable",      "Spindizzy");
    add(&cPhysics,      "Physics",      "Spindizzy");
    add(&cSpinner,      "Spinner",      "Spindizzy");
  }
}
