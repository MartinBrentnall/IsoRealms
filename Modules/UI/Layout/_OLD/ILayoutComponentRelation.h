/*
 * Copyright 2023 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <string>

namespace IsoRealms::UI {
  class LayoutComponent;
  class Layout;

  class ILayoutComponentRelation {
    public:
    virtual void setRelative(const std::string& id, Layout* layout) = 0;
    virtual void setAnchorPosition(float offset) = 0;
    virtual void setOffset(float offset) = 0;
    
    virtual std::string getName(Layout* layout) = 0;
    virtual float getAnchorPosition() = 0;
    virtual float getOffset() = 0;
    
    virtual float getLocation(float aspectRatio) = 0;
    virtual void editOffset(float offset) = 0;
    virtual void renderRelation(float aspectRatio) = 0;
    virtual bool isRelatedTo(LayoutComponent* component) = 0;

    virtual ~ILayoutComponentRelation() {}
  };
}
