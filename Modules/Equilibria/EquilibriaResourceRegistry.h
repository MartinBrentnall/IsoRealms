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
 * You should have received a copy of the GNU General Public License
 *
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <variant>
  
#include "IsoRealms.h"

#include "Modules/Equilibria/Resources/Type/IBoundaryType.h"
#include "Modules/Equilibria/Resources/Type/ICamera.h"
#include "Modules/Equilibria/Resources/Type/IPhysicalObjectType.h"
#include "Modules/Equilibria/Resources/Type/ISurfacePattern.h"
#include "Modules/Equilibria/Resources/Type/IWallPattern.h"
#include "Modules/Equilibria/Resources/Type/IWorldEditorTool.h"
#include "Modules/Equilibria/Resources/Type/IZoneObjectTypeTrait.h"
#include "Modules/Equilibria/Resources/Type/IZoneViewType.h"
  
namespace IsoRealms::Equilibria {
  using ResourceVariant = std::variant<IBoundaryType*,
                                       ICamera*,
                                       IPhysicalObjectType*,
                                       ISurfacePattern*,
                                       IWallPattern*,
                                       IWorldEditorTool*,
                                       IZoneObjectTypeTrait*,
                                       IZoneViewType*>;
  class EquilibriaResourceRegistry : public AbstractResourceRegistry<ResourceVariant, Equilibria> {
    public:
    EquilibriaResourceRegistry(Equilibria& equilibria) :
              AbstractResourceRegistry<ResourceVariant, Equilibria>(equilibria) {
    }
  };
}
  