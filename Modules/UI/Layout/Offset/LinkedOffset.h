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

#include "IsoRealms/IProject.h"

#include "Modules/UI/Layout/ILayoutOffset.h"

namespace IsoRealms::UI {
  class Layout;
  class LayoutComponent;
  
  /**
   * A layout offset linked to the width or height of a component.
   */
  class LinkedOffset : public ILayoutOffset {
    public:
    LinkedOffset(IProject* project, JSONObject object, Layout* layout, LayoutComponent* defaultLink, float defaultRatio);

    /****************************\
     * Implements ILayoutOffset *
    \****************************/
    float getOffset(float aspectRatio) const override;
    void save(JSONObject object, Layout* layout) const override;
    
    private:
    
    // JSON members.    
    static const std::string JSON_LINKED;
    static const std::string JSON_RATIO;
    static const std::string JSON_TYPE;
    static const std::string JSON_VALUE;

    static const std::string TYPE_ABSOLUTE;

    static const std::string VALUE_HEIGHT;
    static const std::string VALUE_WIDTH;

    // Definition data.
    LayoutComponent* cDefLinked;
    bool cDefHorizontal;
    float cDefRatio;
  };
}
