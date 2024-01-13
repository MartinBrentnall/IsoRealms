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

#include "IsoRealms/Persistence/DOMNode.h"

#include "Modules/UI/Layout/ILayoutLocation.h"

namespace IsoRealms::UI {
  
  /**
   * A layout location with a fixed value.
   */
  class AbsoluteLocation : public ILayoutLocation {
    public:
    AbsoluteLocation(float value);
    AbsoluteLocation(DOMNode& node, float defaultValue);
    
    /******************************\
     * Implements ILayoutLocation *
    \******************************/
    float getLocation(float aspectRatio) const override;
    void save(DOMNodeWriter* node, Layout* layout) const override;
    
    private:

    // DOM strings.
    static const std::string ATTRIBUTE_VALUE;
    
    // Definition data.
    float cDefValue; // The location value.
  };
}
