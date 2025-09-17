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
#include "FloatRegistry.h"

#include "IsoRealms/Project/Project.h"

namespace IsoRealms {
  FloatRegistry::FloatRegistry() :
            AssetClientManager(&cLiteral, "Literal") {
  }

  FloatRegistry::Literal::Instance::Instance(Project& project, const float value) :
            cMetadata(project.getApplication().getMetadata("LiteralFloat")),
            cValue(value) {
  }

  float FloatRegistry::Literal::Instance::getValue() const {
    return cValue;
  }

  bool FloatRegistry::Literal::Instance::renderAssetIcon() const {
    return false;
  }

  void FloatRegistry::Literal::Instance::getAssetProperties(PropertyMaker& owner) {
    owner.createPropertyNativeFloat(cMetadata.getPropertyData("Value"), [this]() {return cValue;}, [this](float value) {cValue = value;});
  }

  bool FloatRegistry::Literal::Instance::isDefaultConfiguration() const {
    return cValue == 0.0f;
  }

  void FloatRegistry::Literal::Instance::saveAsset(JSONObject object) const {
    object.addFloat(JSON_VALUE, cValue);
  }

  const std::string FloatRegistry::Literal::Instance::JSON_VALUE = "value";
}
