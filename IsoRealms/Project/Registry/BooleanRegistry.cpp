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
#include "BooleanRegistry.h"

#include "IsoRealms/Project/Project.h"

namespace IsoRealms {
  BooleanRegistry::BooleanRegistry() :
            AssetClientManager(&cLiteral, "Literal") {
  }

  BooleanRegistry::Literal::Instance::Instance(Project& project, bool value) :
            cMetadata(project.getApplication().getMetadata("LiteralBoolean")),
            cValue(value) {
  }

  bool BooleanRegistry::Literal::Instance::getValue() const {
    return cValue;
  }

  bool BooleanRegistry::Literal::Instance::renderAssetIcon() const {
    if (cValue) {
      Utils::renderIconTick();
    } else {
      Utils::renderIconNone();
    }
    return true;
  }

  void BooleanRegistry::Literal::Instance::saveAsset(JSONObject object) const {
    object.addBoolean(JSON_VALUE, cValue);
  }

  void BooleanRegistry::Literal::Instance::getAssetProperties(PropertyMaker& owner) {
    owner.createPropertyNativeBoolean(cMetadata.getPropertyData("Value"), [this]() {return cValue;}, [this](bool value) {cValue = value;});
  }

  bool BooleanRegistry::Literal::Instance::isDefaultConfiguration() const {
    return !cValue;
  }

  const std::string BooleanRegistry::Literal::Instance::JSON_VALUE = "value";
}
