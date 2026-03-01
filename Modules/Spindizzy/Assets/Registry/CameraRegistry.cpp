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
 #include "Modules/Spindizzy/Spindizzy.h"

 #include "CameraRegistry.h"

namespace IsoRealms::Spindizzy {
  CameraRegistry::CameraRegistry(IResourceTypeRegistry& registry) :
            cGameplay(registry.getAssetMetadata("CameraGameplay")),
            cLinked(registry.getAssetMetadata("CameraLinked")),
            cOverview(registry.getAssetMetadata("CameraOverview")),
            cTransitional(registry.getAssetMetadata("CameraTransitional")),
            cVariant(registry.getAssetMetadata("CameraVariant")) {
    add(&cGameplay,     "Gameplay",     "Gameplay");
    add(&cLinked,       "Linked",       "Linked");
    add(&cOverview,     "Overview",     "Overview");
    add(&cTransitional, "Transitional", "Transitional");
    add(&cVariant,      "Variant",      "Variant");
  }
}
