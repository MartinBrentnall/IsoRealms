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
  const std::string AbsoluteLocation::JSON_TYPE     = "type";
  const std::string AbsoluteLocation::JSON_VALUE    = "value";

  const std::string AbsoluteLocation::TYPE_ABSOLUTE = "Absolute";

  AbsoluteLocation::AbsoluteLocation(float value) :
            cDefValue(value) {
  }
    
  AbsoluteLocation::AbsoluteLocation(JSONObject object, float defaultValue) :
            AbsoluteLocation(object.getFloat(JSON_VALUE, defaultValue)) {
  }

  float AbsoluteLocation::getLocation(float aspectRatio) const {
    return cDefValue * aspectRatio;
  }
  
  void AbsoluteLocation::save(JSONObject object, Layout* layout, float defaultValue) const {
    object.addString(JSON_TYPE, TYPE_ABSOLUTE);
    object.addFloat(JSON_VALUE, cDefValue, defaultValue);
  }
}
