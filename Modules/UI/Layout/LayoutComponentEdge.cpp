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
#include "LayoutComponentEdge.h"

#include "Modules/UI/UI.h"

#include "LayoutComponent.h"
#include "Layout.h"

namespace IsoRealms::UI {
  const std::string LayoutComponentEdge::JSON_LOCATION = "location";
  const std::string LayoutComponentEdge::JSON_OFFSET   = "offset";

  LayoutComponentEdge::LayoutComponentEdge(LayoutComponent& parent, float aspectRatio, float value) :
            cDefParent(parent),
            cDefLocation(*this),
            cDefOffset(*this) {
    setLocation(aspectRatio, value);
  }
  
  UI& LayoutComponentEdge::getAssetManager() {
    return cDefParent.getLayout().getUI();
  }

  Project& LayoutComponentEdge::getProject() const {
    return cDefParent.getLayout().getResourceData().getProject();
  }

  bool LayoutComponentEdge::isReadOnly() const {
    return cDefParent.getLayout().getResourceData().isReadOnly();
  }

  void LayoutComponentEdge::setOwner(ProjectFile* owner) {
    cDefParent.getLayout().getResourceData().setOwner(owner);
  }

  LayoutComponentEdge::LayoutComponentEdge(LayoutComponent& parent, JSONObject object, const std::string& tag) :
            LayoutComponentEdge(parent, 1.0f, 0.0f) {
    JSONObject mEdgeObject = object.getObject(tag);
    if (mEdgeObject.hasMember(JSON_LOCATION)) {
      cDefLocation.init(mEdgeObject, JSON_LOCATION);
    }
    if (mEdgeObject.hasMember(JSON_OFFSET)) {
      cDefOffset.init(mEdgeObject, JSON_OFFSET);
    }
  }

  void LayoutComponentEdge::setLocation(float aspectRatio, float value) {
    cDefLocation->setAbsolute(aspectRatio, value - cDefOffset->getOffset(aspectRatio));
  }

  void LayoutComponentEdge::setOffset(float aspectRatio, float value) {
    cDefOffset->setAbsolute(aspectRatio, value - cDefLocation->getLocation(aspectRatio));
  }

  LayoutComponent& LayoutComponentEdge::getComponent() {
    return cDefParent;
  }

  bool LayoutComponentEdge::isHorizontalEdge() const {
    return cDefParent.isHorizontalEdge(*this);
  }
  
  bool LayoutComponentEdge::isPositiveEdge() const {
    return cDefParent.isPositiveEdge(*this);
  }

  float LayoutComponentEdge::getLocation(float aspectRatio) const {
    return cDefLocation->getLocation(aspectRatio) + cDefOffset->getOffset(aspectRatio);
  }
  
  void LayoutComponentEdge::save(JSONObject object, const std::string& tag, Layout* layout, float defaultValue) const {
    JSONObject mEdgeObject = object.addObject(tag);
    cDefLocation.save(mEdgeObject, JSON_LOCATION);
    cDefOffset.save(mEdgeObject, JSON_OFFSET);
  }
  
  void LayoutComponentEdge::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyTreeSelector(metadata.getPropertyData("ComponentEdgeLocation"), cDefLocation);
    owner.createPropertyTreeSelector(metadata.getPropertyData("ComponentEdgeOffset"),   cDefOffset);
  }

  void LayoutComponentEdge::renderRelation(float aspectRatio) const {
    cDefLocation->renderRelation(aspectRatio);
    cDefOffset->renderRelation(aspectRatio);
  }
}
