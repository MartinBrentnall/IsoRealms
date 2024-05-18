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
#include "AbsoluteOffset.h"

namespace IsoRealms::UI {
  const std::string AbsoluteOffset::JSON_TYPE  = "type";
  const std::string AbsoluteOffset::JSON_VALUE = "value";

  const std::string AbsoluteOffset::TYPE_ABSOLUTE = "Absolute";
  
  AbsoluteOffset::AbsoluteOffset(float value) :
            cDefValue(value) {
  }
  
  AbsoluteOffset::AbsoluteOffset(JSONObject object) :
            AbsoluteOffset(object.getFloat(JSON_VALUE)) {
  }

  float AbsoluteOffset::getOffset(float aspectRatio) const {
    return cDefValue;
  }

  void AbsoluteOffset::save(JSONObject object, Layout* layout) const {
    object.addString(JSON_TYPE, TYPE_ABSOLUTE);
    object.addFloat(JSON_VALUE, cDefValue);
  }
}
