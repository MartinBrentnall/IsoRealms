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
#include "LayoutOffsetAbsolute.h"

namespace IsoRealms::UI {
  const std::string LayoutOffsetAbsolute::JSON_VALUE = "value";

  LayoutOffsetAbsolute::LayoutOffsetAbsolute(IProject& project, LayoutComponentEdge& owner) :
            cDefValue(0.0f) {
  }

  LayoutOffsetAbsolute::LayoutOffsetAbsolute(IProject& project, LayoutComponentEdge& owner, JSONObject object) :
            cDefValue(object.getFloat(JSON_VALUE)) {
  }

  float LayoutOffsetAbsolute::getOffset(float aspectRatio) const {
    return cDefValue;
  }

  void LayoutOffsetAbsolute::setAbsolute(float aspectRatio, float value) {
    // TODO: Implement this.
  }

  void LayoutOffsetAbsolute::renderRelation(float aspectRatio) const {
    // Nothing to do.
  }

  bool LayoutOffsetAbsolute::renderAssetIcon() const {
    return false;
  }
  
  void LayoutOffsetAbsolute::saveAsset(JSONObject object) const {
    object.addFloat(JSON_VALUE, cDefValue);
  }
  
  void LayoutOffsetAbsolute::getAssetProperties(PropertyMaker& owner) {
    owner.createPropertyNativeFloat("Value", [this]() {return cDefValue;}, [this](float value) {cDefValue = value; return true;});
  }

  bool LayoutOffsetAbsolute::isDefaultConfiguration() const {
    return false; // TODO: Implement this.
  }
}
