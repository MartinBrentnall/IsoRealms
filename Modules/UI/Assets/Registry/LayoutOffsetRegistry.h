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

#include "IsoRealms.h"

#include "Modules/UI/Assets/Fixed/LayoutOffset/LayoutOffsetAbsolute.h"
#include "Modules/UI/Assets/Fixed/LayoutOffset/LayoutOffsetLinked.h"
#include "Modules/UI/Assets/Type/ILayoutOffset.h"
#include "Modules/UI/Layout/LayoutComponentEdge.h"

namespace IsoRealms::UI {
  class LayoutComponentEdge;

  class LayoutOffsetRegistry : public AssetClientManager<LayoutOffsetRegistry, LayoutComponentEdge, ILayoutOffset> {
    public:
    LayoutOffsetRegistry(IComponentTypeRegistry& registry);

    private:
    AssetInstanced<LayoutComponentEdge, ILayoutOffset, LayoutOffsetAbsolute> cAbsolute;
    AssetInstanced<LayoutComponentEdge, ILayoutOffset, LayoutOffsetLinked>   cLinked;
  };
}
