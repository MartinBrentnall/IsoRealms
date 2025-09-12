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
#include "StringRegistry.h"

#include "IsoRealms/Project/Project.h"

namespace IsoRealms {
  StringRegistry::StringRegistry(Project& project) :
            AssetClientManager(&cLiteral, "Literal") {

    // Support conversions.
    add(&cFloats,   ":Float",   "Conversions");
    add(&cIntegers, ":Integer", "Conversions");
  }

  StringRegistry::Literal::Instance::Instance(Project& project, const std::string& value) :
            cMetadata(project.getApplication().getMetadata("LiteralString")),
            cValue(value) {
  }

  void StringRegistry::Literal::Instance::setValue(const std::string& value) {
    cValue = value;
  }

  std::string StringRegistry::Literal::Instance::getValue() const {
    return cValue;
  }

  bool StringRegistry::Literal::Instance::renderAssetIcon() const {
    return false;
  }

  void StringRegistry::Literal::Instance::getAssetProperties(PropertyMaker& owner) {
    owner.createPropertyNativeString(cMetadata.getPropertyData("Value"), [this]() {return cValue;}, [this](const std::string& value) {cValue = value;});
  }

  bool StringRegistry::Literal::Instance::isDefaultConfiguration() const {
    return cValue == "";
  }

  void StringRegistry::Literal::Instance::saveAsset(JSONObject object) const {
    object.addString(JSON_VALUE, cValue);
  }

  const std::string StringRegistry::Literal::Instance::JSON_VALUE = "value";
}
