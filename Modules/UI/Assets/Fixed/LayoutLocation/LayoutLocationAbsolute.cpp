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
#include "LayoutLocationAbsolute.h"

#include "Modules/UI/Layout/LayoutComponentEdge.h"

namespace IsoRealms::UI {
  const std::string LayoutLocationAbsolute::JSON_VALUE = "value";

  LayoutLocationAbsolute::LayoutLocationAbsolute(IProject& project, LayoutComponentEdge& owner) :
            cParent(owner),
            cDefValue(owner.isPositiveEdge() ? 1.0f : -1.0f) {
  }
  
  LayoutLocationAbsolute::LayoutLocationAbsolute(IProject& project, LayoutComponentEdge& owner, JSONObject object) :
            cParent(owner),
            cDefValue(object.getFloat(JSON_VALUE, owner.isPositiveEdge() ? 1.0f : -1.0f)) {
  }

  float LayoutLocationAbsolute::getLocation(float aspectRatio) const {
    return cDefValue * aspectRatio;
  }
  
  void LayoutLocationAbsolute::setAbsolute(float aspectRatio, float value) {
    cDefValue = value / aspectRatio;
  }
  
  void LayoutLocationAbsolute::renderRelation(float aspectRatio) const {
    // Nothing to do.
  }

  bool LayoutLocationAbsolute::renderAssetIcon() const {
    return false;
  }
  
  void LayoutLocationAbsolute::saveAsset(JSONObject object) const {
    object.addFloat(JSON_VALUE, cDefValue, cParent.isPositiveEdge() ? 1.0f : -1.0f);
  }
  
  std::vector<std::unique_ptr<IProperty>> LayoutLocationAbsolute::getAssetProperties(IPropertyOwner& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>(PropertyData("Value", "TODO"), [this]() {return cDefValue;}, [this](float value) {cDefValue = value; return true;}));
    return mProperties;
  }

  bool LayoutLocationAbsolute::isDefaultConfiguration() const {
    return false; // TODO: Implement this.
  }
}
