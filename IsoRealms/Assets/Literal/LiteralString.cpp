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
#include "LiteralString.h"

#include "IsoRealms/Editing/Property/PropertyNativeString.h"
#include "IsoRealms/Project.h"
#include "IsoRealms/PropertyMaker.h"

namespace IsoRealms {
  LiteralString::LiteralString(Project& project, const std::string& value) :
            cMetadata(project.getApplication().getMetadata("LiteralString")),
            cValue(value) {
  }

  void LiteralString::setValue(const std::string& value) {
    cValue = value;
  }

  std::string LiteralString::getValue() const {
    return cValue;
  }

  bool LiteralString::renderAssetIcon() const {
    return false;
  }

  void LiteralString::getAssetProperties(PropertyMaker& owner) {
    owner.createPropertyNativeString(cMetadata.getPropertyData("Value"), [this]() {return cValue;}, [this](const std::string& value) {cValue = value;});
  }

  bool LiteralString::isDefaultConfiguration() const {
    return cValue == "";
  }

  void LiteralString::saveAsset(JSONObject object) const {
    object.addString(JSON_VALUE, cValue);
  }

  const std::string LiteralString::JSON_VALUE = "value";
}
