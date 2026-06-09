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
#include "Metadata.h"

#include "Persistence/JSONObject.h"
#include "Persistence/JSONThing.h"
#include "PropertyData.h"

namespace IsoRealms {
  Metadata::Metadata() :
            cParent(nullptr) {
  }

  void Metadata::load(JSONObject object) {
    for (JSONThing mPropertiesThing : object) {
      std::string mPropertyID = mPropertiesThing.getName();
      JSONObject mPropertyObject = mPropertiesThing.getValue();
      cPropertyHelp.emplace(mPropertyID, std::make_unique<PropertyData>(mPropertyObject));
    }
  }

  void Metadata::setParent(const Metadata* parent) {
    cParent = parent;
  }
  
  const PropertyData Metadata::getPropertyData(const std::string& key) const {
    std::map<std::string, std::unique_ptr<PropertyData>>::const_iterator mIterator = cPropertyHelp.find(key);
    if (mIterator != cPropertyHelp.end()) {
      return *mIterator->second;
    }
    if (cParent != nullptr) {
      return cParent->getPropertyData(key);
    }
    return PropertyData("TODO: Missing property for \"" + key + "\"", "TODO: Missing description for \"" + key + "\"");
  }
}
