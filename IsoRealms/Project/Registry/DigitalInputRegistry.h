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

#include "IsoRealms/Resources/Type/IDigitalInput.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/Resources/Fixed/DigitalInput/GameControllerButton.h"
#include "IsoRealms/Resources/Fixed/DigitalInput/GameControllerHat.h"
#include "IsoRealms/Resources/Fixed/DigitalInput/KeyboardKey.h"
#include "IsoRealms/Resources/Fixed/DigitalInput/MouseButton.h"
#include "IsoRealms/Resources/Providers/ResourceInstanced.h"

#include "ResourceClientManager.h"

namespace IsoRealms {
  class DigitalInputRegistry : public ResourceClientManager<DigitalInputRegistry, IComponentData, IDigitalInput> {
    public:
    DigitalInputRegistry(Project& project);
 
    private:
    ResourceInstanced<IComponentData, IDigitalInput, GameControllerButton> cGameControllerButton;
    ResourceInstanced<IComponentData, IDigitalInput, GameControllerHat>    cGameControllerHat;
    ResourceInstanced<IComponentData, IDigitalInput, MouseButton>          cMouseButton;
    ResourceInstanced<IComponentData, IDigitalInput, KeyboardKey>          cKeyboardKey;
  };
}
 