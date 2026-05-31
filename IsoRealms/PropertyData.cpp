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
#include "PropertyData.h"

#include "Persistence/JSONObject.h"

namespace IsoRealms {
  PropertyData::PropertyData(const std::string& name, const std::string& tooltip) :
            cName(name),
            cTooltip(tooltip) {
  }

  PropertyData::PropertyData(JSONObject object) :
            cName(object.getString(JSON_NAME)),
            cTooltip(object.getString(JSON_DESCRIPTION)) {
  }

  PropertyData::PropertyData(const PropertyData& other) :
            cName(other.cName),
            cTooltip(other.cTooltip) {
  }

  std::string PropertyData::getName() const {
    return cName;
  }

  std::string PropertyData::getTooltip() const {
    return cTooltip;
  }
}

