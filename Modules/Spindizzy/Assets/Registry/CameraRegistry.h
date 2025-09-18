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

#include "Modules/Spindizzy/Assets/Fixed/Camera/CameraGameplay.h"
#include "Modules/Spindizzy/Assets/Fixed/Camera/CameraLinked.h"
#include "Modules/Spindizzy/Assets/Fixed/Camera/CameraOverview.h"
#include "Modules/Spindizzy/Assets/Fixed/Camera/CameraTransitional.h"
#include "Modules/Spindizzy/Assets/Fixed/Camera/CameraVariant.h"
#include "Modules/Spindizzy/Assets/Type/ICamera.h"
#include "Modules/Spindizzy/WorldView/WorldView.h"

namespace IsoRealms::Spindizzy {
  class CameraRegistry : public AssetClientManager<CameraRegistry, WorldView, ICamera> {
    public:
    CameraRegistry(IResourceTypeRegistry& registry);

    private:
    AssetInstanced<WorldView, ICamera, CameraGameplay>     cGameplay;
    AssetInstanced<WorldView, ICamera, CameraLinked>       cLinked;
    AssetInstanced<WorldView, ICamera, CameraOverview>     cOverview;
    AssetInstanced<WorldView, ICamera, CameraTransitional> cTransitional;
    AssetInstanced<WorldView, ICamera, CameraVariant>      cVariant;
  };
}
