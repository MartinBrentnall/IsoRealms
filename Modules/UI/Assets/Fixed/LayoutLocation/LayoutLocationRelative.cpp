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
#include "LayoutLocationRelative.h"

#include "Modules/UI/Layout/Layout.h"
#include "Modules/UI/Layout/LayoutComponent.h"
#include "Modules/UI/UI.h"

namespace IsoRealms::UI {
  LayoutLocationRelative::LayoutLocationRelative(const Metadata& metadata, LayoutComponentEdge& owner) :
            cParent(owner),
            cDefRelative(nullptr),
            cDefValue(0.0f) {
  }
  
  LayoutLocationRelative::LayoutLocationRelative(const Metadata& metadata, LayoutComponentEdge& owner, JSONObject object) :
            cParent(owner),
            cDefRelative(nullptr),
            cDefValue(object.getFloat(JSON_VALUE, owner.isPositiveEdge() ? 1.0f : -1.0f)) {
    cDefRelative = cParent.getComponent().getLayout().getComponent(object.getString(JSON_RELATIVE));
  }

  float LayoutLocationRelative::getLocation(float aspectRatio) const {
    float mComponentStart = cParent.isHorizontalEdge() ? cDefRelative->getLeft(aspectRatio)  : cDefRelative->getBottom();
    float mComponentEnd   = cParent.isHorizontalEdge() ? cDefRelative->getRight(aspectRatio) : cDefRelative->getTop();
    return mComponentStart + ((mComponentEnd - mComponentStart) / 2.0f) * (cDefValue + 1.0f);
  }
  
  void LayoutLocationRelative::setAbsolute(float aspectRatio, float value) {
    float mComponentStart = cParent.isHorizontalEdge() ? cDefRelative->getLeft(aspectRatio)  : cDefRelative->getBottom();
    float mComponentEnd   = cParent.isHorizontalEdge() ? cDefRelative->getRight(aspectRatio) : cDefRelative->getTop();
    cDefValue = ((value - mComponentStart) / (mComponentEnd - mComponentStart) * 2.0f - 1.0f);
  }
  
  void LayoutLocationRelative::renderRelation(float aspectRatio) const {
    cDefRelative->renderAsRelation(aspectRatio);
  }

  bool LayoutLocationRelative::renderAssetIcon() const {
    return false;
  }
  
  void LayoutLocationRelative::saveAsset(JSONObject object) const {
    object.addFloat(JSON_VALUE, cDefValue, cParent.isPositiveEdge() ? 1.0f : -1.0f);
    object.addString(JSON_RELATIVE, cParent.getComponent().getLayout().getName(cDefRelative));
  }
  
  void LayoutLocationRelative::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyNativeFloat(JSON_VALUE,    [this]() {return cDefValue;}, [this](float value) {cDefValue = value;});
    owner.createPropertyList(       JSON_RELATIVE, cParent.getComponent().getAvailableComponentNames(), [this]() {return cParent.getComponent().getLayout().getName(cDefRelative);}, [this](const std::string& value) {std::cout << "TODO: Support setting relative component!" << std::endl;});
  }

  bool LayoutLocationRelative::isDefaultConfiguration() const {
    return false; // TODO: Implement this.
  }
}
