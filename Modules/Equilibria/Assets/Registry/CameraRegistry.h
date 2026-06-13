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

#include "Modules/Equilibria/Assets/Fixed/Camera/CameraGameplay.h"
#include "Modules/Equilibria/Assets/Fixed/Camera/CameraLinked.h"
#include "Modules/Equilibria/Assets/Fixed/Camera/CameraOverview.h"
#include "Modules/Equilibria/Assets/Fixed/Camera/CameraTransitional.h"
#include "Modules/Equilibria/Assets/Fixed/Camera/CameraVariant.h"
#include "Modules/Equilibria/Assets/Type/ICamera.h"
#include "Modules/Equilibria/WorldView/WorldView.h"

namespace IsoRealms::Equilibria {
  class CameraRegistry : public AssetClientManager<CameraRegistry, WorldView, ICamera> {
    public:
    CameraRegistry(IComponentTypeRegistry& registry);

    private:
    AssetInstanced<WorldView, ICamera, CameraGameplay>     cGameplay;
    AssetInstanced<WorldView, ICamera, CameraLinked>       cLinked;
    AssetInstanced<WorldView, ICamera, CameraOverview>     cOverview;
    AssetInstanced<WorldView, ICamera, CameraTransitional> cTransitional;
    AssetInstanced<WorldView, ICamera, CameraVariant>      cVariant;
  };
}
