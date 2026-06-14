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

#include "Modules/UI/Resources/Type/ILayoutOffset.h"

namespace IsoRealms::UI {
  class LayoutComponentEdge;

  class LayoutOffset : public Resource<LayoutOffset, ILayoutOffset, LayoutComponentEdge> {
    public:
    LayoutOffset(LayoutComponentEdge& owner);

    /*************************************************************************\
     * Implements Resource<LayoutOffset, ILayoutOffset, LayoutComponentEdge> *
    \*************************************************************************/
    ILayoutOffset* createLiteralResource(LayoutComponentEdge& edge);
  };
}
