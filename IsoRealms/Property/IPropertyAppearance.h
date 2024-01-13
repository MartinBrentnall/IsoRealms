/*
 * Copyright 2023 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <string>

#include "IsoRealms/Assets/Type/IFont.h"

namespace IsoRealms {
  class IPropertyAppearance {
    public:
    virtual void print(const std::string& value, const float lineOffset, float xOffset) const = 0;
    virtual void print(float x, float y, float scale, IFont::Alignment alignment, const std::string& value) const = 0;
    virtual float getWidth(const std::string& value) const = 0;
    virtual float getIconWidth() const = 0;
    virtual float getLineHeight() const = 0;
    virtual float getSelectionHighlightHeight() const = 0;
    virtual float getIconTextSpacing() const = 0;
    virtual float getLabelPropertySpacing() const = 0;
    virtual void transformForIcon(const float lineOffset) const = 0;
    virtual float getY(const float lineOffset) const = 0;
    virtual float getScale() const = 0;

    virtual ~IPropertyAppearance() {
    }
  };
}
