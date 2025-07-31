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
#include "LayoutOffsetLinked.h"

#include "Modules/UI/Layout/Layout.h"
#include "Modules/UI/Layout/LayoutComponent.h"
#include "Modules/UI/UI.h"

namespace IsoRealms::UI {
  const std::string LayoutOffsetLinked::JSON_LINKED = "linked";
  const std::string LayoutOffsetLinked::JSON_RATIO  = "ratio";
  const std::string LayoutOffsetLinked::JSON_VALUE  = "value";

  const std::string LayoutOffsetLinked::VALUE_HEIGHT = "Height";
  const std::string LayoutOffsetLinked::VALUE_WIDTH  = "Width";
  
  LayoutOffsetLinked::LayoutOffsetLinked(const Metadata& metadata, LayoutComponentEdge& owner) :
            cMetadata(metadata),
            cParent(owner),
            cDefLinked(nullptr),
            cDefHorizontal(false),
            cDefRatio(0.0f) {
  }
  
  LayoutOffsetLinked::LayoutOffsetLinked(const Metadata& metadata, LayoutComponentEdge& owner, JSONObject object) :
            cMetadata(metadata),
            cParent(owner),
            cDefLinked(nullptr),
            cDefHorizontal(object.getString(JSON_VALUE) == VALUE_WIDTH),
            cDefRatio(object.getFloat(JSON_RATIO, owner.isPositiveEdge() ? 1.0f : -1.0f)) {
    std::string mLinkedName = object.getString(JSON_LINKED);
    cDefLinked = mLinkedName == "" ? &cParent.getComponent() : cParent.getComponent().getLayout().getComponent(mLinkedName);
  }

  float LayoutOffsetLinked::getOffset(float aspectRatio) const {
    float mComponentStart = cDefHorizontal ? cDefLinked->getLeft(aspectRatio)  : cDefLinked->getBottom();
    float mComponentEnd   = cDefHorizontal ? cDefLinked->getRight(aspectRatio) : cDefLinked->getTop();
    return (mComponentEnd - mComponentStart) * cDefRatio;
  }
  
  void LayoutOffsetLinked::renderRelation(float aspectRatio) const {
    cDefLinked->renderAsRelation(aspectRatio);
  }

  void LayoutOffsetLinked::setAbsolute(float aspectRatio, float value) {
    // TODO: Implement this.
  }

  bool LayoutOffsetLinked::renderAssetIcon() const {
    return false;
  }
  
  void LayoutOffsetLinked::saveAsset(JSONObject object) const {
    object.addString(JSON_VALUE,  cDefHorizontal ? VALUE_WIDTH : VALUE_HEIGHT);
    object.addString(JSON_LINKED, cParent.getComponent().getLayout().getName(cDefLinked));
    object.addFloat(JSON_RATIO,   cDefRatio, cParent.isPositiveEdge() ? 1.0f : -1.0f);   
  }
  
  void LayoutOffsetLinked::getAssetProperties(PropertyMaker& owner) {
//    owner.createPropertyList(cMetadata.getPropertyData("Orientation"), {VALUE_WIDTH, VALUE_HEIGHT}, [this]() {return cDefHorizontal ? VALUE_WIDTH : VALUE_HEIGHT;}, [this](const std::string& value) {cDefHorizontal = value == VALUE_WIDTH;}));
    owner.createPropertyList(       cMetadata.getPropertyData("LinkedTo"), cParent.getComponent().getAvailableComponentNames(), [this]() {return cParent.getComponent().getLayout().getName(cDefLinked);}, [this](const std::string& value) {std::cout << "TODO: Support setting linked component!" << std::endl;});
    owner.createPropertyNativeFloat(cMetadata.getPropertyData("Ratio"),    [this]() {return cDefRatio;}, [this](float value) {cDefRatio = value;});
  }

  bool LayoutOffsetLinked::isDefaultConfiguration() const {
    return false; // TODO: Implement this.
  }
}
