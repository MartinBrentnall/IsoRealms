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
  LayoutOffsetLinked::LayoutOffsetLinked(LayoutComponentEdge& owner) :
            cParent(owner) {
  }

  float LayoutOffsetLinked::getOffset(float aspectRatio) const {
    float mComponentStart = cDefHorizontal ? cDefLinked->getLeft(aspectRatio)  : cDefLinked->getBottom();
    float mComponentEnd   = cDefHorizontal ? cDefLinked->getRight(aspectRatio) : cDefLinked->getTop();
    return (mComponentEnd - mComponentStart) * cDefRatio;
  }
  
  void LayoutOffsetLinked::renderRelation(float aspectRatio) const {
    cDefLinked->renderAsRelation(aspectRatio);
  }

  void LayoutOffsetLinked::setAbsolute(float offset, float aspectRatio) {
    float mComponentStart = cDefHorizontal ? cDefLinked->getLeft(aspectRatio) : cDefLinked->getBottom();
    float mComponentEnd = cDefHorizontal ? cDefLinked->getRight(aspectRatio) : cDefLinked->getTop();
    float mDifference = mComponentEnd - mComponentStart;
    cDefRatio = mDifference != 0.0f ? offset / mDifference : 0.0f;
  }
  
  void LayoutOffsetLinked::defineResource(IComponentDefiner& definer) {
// TODO   definer.propertyList("Orientation", {VALUE_WIDTH, VALUE_HEIGHT}, [this]() {return cDefHorizontal ? VALUE_WIDTH : VALUE_HEIGHT;}, [this](const std::string& value) {cDefHorizontal = value == VALUE_WIDTH;}));
    definer.propertyList( "linked", cParent.getComponent().getAvailableComponentNames(), [this]() {return cParent.getComponent().getLayout().getName(cDefLinked);}, [this](const std::string& value) {cDefLinked = value.empty() ? &cParent.getComponent() : cParent.getComponent().getLayout().getComponent(value);});
    definer.propertyFloat("ratio",  [this]() {return cDefRatio;}, [this](float value) {cDefRatio = value;}, cParent.isPositiveEdge() ? 1.0f : -1.0f);
  }

  bool LayoutOffsetLinked::isDefaultConfiguration() const {
    return false; // TODO: Implement this.
  }
}
