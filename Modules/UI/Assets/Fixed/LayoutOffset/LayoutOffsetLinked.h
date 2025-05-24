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

#include "IsoRealms/Editing.h"
#include "IsoRealms/IProject.h"

#include "Modules/UI/Assets/Type/ILayoutOffset.h"

namespace IsoRealms::UI {
  class LayoutComponent;
  class LayoutComponentEdge;
  
  /**
   * A layout offset linked to the width or height of a component.
   */
  class LayoutOffsetLinked : public ILayoutOffset {
    public:
    LayoutOffsetLinked(IProject& project, LayoutComponentEdge& owner);
    LayoutOffsetLinked(IProject& project, LayoutComponentEdge& owner, JSONObject object);

    /****************************\
     * Implements ILayoutOffset *
    \****************************/
    float getOffset(float aspectRatio) const override;
    void setAbsolute(float aspectRatio, float value) override;
    void renderRelation(float aspectRatio) const override;

    /***************************************\
     * Implements IAsset via ILayoutOffset *
    \***************************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:
    
    // JSON members.    
    static const std::string JSON_LINKED;
    static const std::string JSON_RATIO;
    static const std::string JSON_VALUE;

    static const std::string VALUE_HEIGHT;
    static const std::string VALUE_WIDTH;

    // External interfaces.
    LayoutComponentEdge& cParent;

    // Definition data.
    LayoutComponent* cDefLinked;
    bool cDefHorizontal;
    float cDefRatio;
  };
}
