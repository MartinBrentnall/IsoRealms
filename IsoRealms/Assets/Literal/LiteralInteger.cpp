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
#include "LiteralInteger.h"

#include "IsoRealms/Editing/Property/IPropertyManager.h"
#include "IsoRealms/Editing/Property/PropertyNativeInteger.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Project.h"

namespace IsoRealms {
  LiteralInteger::LiteralInteger(Project& project, const int value):
          cMetadata(project.getApplication().getMetadata("LiteralInteger")),
          cValue(value) {
    // Nothing to do.
  }  

  int LiteralInteger::getValue() const {
    return cValue;
  }
  
  bool LiteralInteger::renderAssetIcon() const {
    return false;
  }

  void LiteralInteger::saveAsset(JSONObject object) const {
    object.addInteger(JSON_VALUE, cValue);
  }

  void LiteralInteger::getAssetProperties(PropertyMaker& owner) {
    owner.createPropertyNativeInteger(cMetadata.getPropertyData("Value"), [this]() {return cValue;}, [this](int value) {cValue = value;});
  }

  bool LiteralInteger::isDefaultConfiguration() const {
    return cValue == 0;
  }

  const std::string LiteralInteger::JSON_VALUE = "value";
}
