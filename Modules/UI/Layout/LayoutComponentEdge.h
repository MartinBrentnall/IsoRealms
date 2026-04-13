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
#pragma once

#include <map>
#include <string>

#include "IsoRealms.h"

#include "Modules/UI/Assets/Client/LayoutLocation.h"
#include "Modules/UI/Assets/Client/LayoutOffset.h"

namespace IsoRealms::UI {
  class Layout;
  class LayoutComponent;
  class UI;

  class LayoutComponentEdge {
    public:
    LayoutComponentEdge(LayoutComponent& parent, float aspectRatio, float value);
    LayoutComponentEdge(LayoutComponent& parent, JSONObject object, const std::string& tag);

    /***************************\
     * Asset client interfaces *
    \***************************/
    UI& getAssetManager();
    Project& getProject() const;
    bool isReadOnly() const;
    void setOwner(ProjectFile* owner);

    void setLocation(float aspectRatio, float value);
    void setOffset(float aspectRatio, float value);
    LayoutComponent& getComponent();
    bool isHorizontalEdge() const;
    bool isPositiveEdge() const;
    float getLocation(float aspectRatio) const;
    void save(JSONObject object, const std::string& tag, Layout* layout, float defaultValue) const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void renderRelation(float aspectRatio) const;
    
    private:
    static const std::string JSON_LOCATION;
    static const std::string JSON_OFFSET;
   
    // External interfaces.
    LayoutComponent& cDefParent;
    
    // Definition data.
    LayoutLocation cDefLocation;
    LayoutOffset cDefOffset;
  };
}
