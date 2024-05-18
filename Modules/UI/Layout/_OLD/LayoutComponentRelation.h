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

#include "LayoutComponent.h"
#include "ILayoutComponentRelation.h"

namespace IsoRealms::UI {
  class LayoutComponentRelation : public ILayoutComponentRelation {
    public:
    LayoutComponentRelation(LayoutComponent*, float, float, bool);
      
    /***************************************\
     * Implements ILayoutComponentRelation *
    \***************************************/
    void setRelative(const std::string& name, Layout* layout) override;
    void setAnchorPosition(float offset) override;
    float getAnchorPosition() override;
    float getOffset() override;
    void setOffset(float) override;
    float getLocation(float aspectRatio) override;
    void editOffset(float) override;
    std::string getName(Layout*) override;
    void renderRelation(float aspectRatio) override;
    bool isRelatedTo(LayoutComponent* component) override;
    
    private:
    LayoutComponent* cRelative;
    float cPosition;
    float cOffset;
    bool cHorizontal;
    
    float getEdgeLocation();
  };
}
