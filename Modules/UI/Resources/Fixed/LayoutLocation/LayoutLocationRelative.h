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

#include "Modules/UI/Resources/Type/ILayoutLocation.h"

namespace IsoRealms::UI {
  class LayoutComponent;
  class LayoutComponentEdge;

  /**
   * A layout location related to an existing component.
   */
  class LayoutLocationRelative : public ILayoutLocation {
    public:
    LayoutLocationRelative(const Metadata& metadata, LayoutComponentEdge& owner);
    
    /******************************\
     * Implements ILayoutLocation *
    \******************************/
    void defineResource(IComponentDefiner& definer) override;
    bool isDefaultConfiguration() const override;
    float getLocation(float aspectRatio) const override;
    void setAbsolute(float aspectRatio, float value) override;
    void renderRelation(float aspectRatio) const override;

    private:

    // External interfaces.
    LayoutComponentEdge& cParent;
    
    // Definition data.
    LayoutComponent* cDefRelative = nullptr; /// Component to which this location is related.
    float cDefValue = 0.0f;                  /// Relative position to the related component, from -1.0f to +1.0f.
  };
}
