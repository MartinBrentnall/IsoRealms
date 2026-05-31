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

#include "IsoRealms/Assets/Type/IDigitalInput.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Assets/Fixed/DigitalInput/GameControllerButton.h"
#include "IsoRealms/Assets/Fixed/DigitalInput/GameControllerHat.h"
#include "IsoRealms/Assets/Fixed/DigitalInput/KeyboardKey.h"
#include "IsoRealms/Assets/Fixed/DigitalInput/MouseButton.h"
#include "IsoRealms/Assets/Providers/AssetInstanced.h"

#include "AssetClientManager.h"

namespace IsoRealms {
  class DigitalInputRegistry : public AssetClientManager<DigitalInputRegistry, IResourceData, IDigitalInput> {
    public:
    DigitalInputRegistry(Project& project);
 
    private:
    AssetInstanced<IResourceData, IDigitalInput, GameControllerButton> cGameControllerButton;
    AssetInstanced<IResourceData, IDigitalInput, GameControllerHat>    cGameControllerHat;
    AssetInstanced<IResourceData, IDigitalInput, MouseButton>          cMouseButton;
    AssetInstanced<IResourceData, IDigitalInput, KeyboardKey>          cKeyboardKey;
  };
}
 