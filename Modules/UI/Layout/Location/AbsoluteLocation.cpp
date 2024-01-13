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
#include "AbsoluteLocation.h"

namespace IsoRealms::UI {
  const std::string AbsoluteLocation::ATTRIBUTE_VALUE = "value";
  
  AbsoluteLocation::AbsoluteLocation(float value) :
            cDefValue(value) {
  }
    
  AbsoluteLocation::AbsoluteLocation(DOMNode& node, float defaultValue) :
            AbsoluteLocation(node.getFloatAttribute(ATTRIBUTE_VALUE, defaultValue)) {
  }
    
  float AbsoluteLocation::getLocation(float aspectRatio) const {
    return cDefValue * aspectRatio;
  }
  
  void AbsoluteLocation::save(DOMNodeWriter* node, Layout* layout) const {
    node->addAttribute(ATTRIBUTE_VALUE, cDefValue);
  }  
}
