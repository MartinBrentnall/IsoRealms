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

#include "IsoRealms/AnimatedFloat.h"
#include "IsoRealms/Property/IProperty.h"
#include "IsoRealms/Property/IPropertyAppearance.h"
#include "IsoRealms/ResourceDefinition.h"

namespace IsoRealms {
  class PropertyAppearance : public IPropertyAppearance {
    public:
    void set(IFont* font, float scale);

    /**********************************\
     * Implements IPropertyAppearance *
    \**********************************/
    void print(const std::string& value, const float lineOffset, float xOffset) const override;
    void print(float x, float y, float scale, IFont::Alignment alignment, const std::string& value) const override;
    float getWidth(const std::string& value) const override;
    float getIconWidth() const override;
    float getLineHeight() const override;
    float getSelectionHighlightHeight() const override;
    float getIconTextSpacing() const override;
    float getLabelPropertySpacing() const override;
    void transformForIcon(const float lineOffset) const override;
    float getY(const float lineOffse) const override;
    float getScale() const override;
    
    private:
    
    // Definition data.
    IFont* cDefFont; /// Font for property rendering.
    float cDefScale; /// Property scaling.
  };
}
