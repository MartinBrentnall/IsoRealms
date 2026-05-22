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
#include <string>

#include "IsoRealms/Types.h"

#include "UISignalID.h"

namespace IsoRealms {
  class IMenuItem {
    public:
    virtual ~IMenuItem() = default;
    
    virtual float getWidth(IUIStyle& style) const = 0;
    virtual float getHeight(IUIStyle& style) const = 0;
    virtual float getIndentation(IUIStyle& style) const = 0;
    virtual void render(IUIStyle& style, float y, float aspectRatio) const = 0;
    virtual bool input(UISignalID id) = 0;
    virtual std::string getTooltip() const = 0;
    virtual bool isSelectable() const = 0;
  };
}
 