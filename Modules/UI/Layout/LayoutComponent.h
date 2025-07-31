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

#include "LayoutComponentEdge.h"

namespace IsoRealms::UI {
  class Layout;

  class LayoutComponent final {
    public:
    LayoutComponent(IProject& project, Layout& layout, float x1, float y1, float x2, float y2, float aspectRatio);
    LayoutComponent(IProject& project, Layout& layout, JSONObject object);
    void reset();
    void registerAssets(ResourceAssetRegistry& assets, const std::string& name);
    void render(float scale, float aspectRatio);
    void renderEditor(float scale, float aspectRatio);
    void save(JSONObject object) const;
//    bool pickHandle(float x, float y, float scale, float aspectRatio);
//    bool move(float x, float y, float aspectRatio);
    void setLeftEdgeLocation(float value, float aspectRatio);
    void setRightEdgeLocation(float value, float aspectRatio);
    void setBottomEdgeLocation(float value);
    void setTopEdgeLocation(float value);

    void setLeftEdgeOffset(float value, float aspectRatio);
    void setRightEdgeOffset(float value, float aspectRatio);
    void setBottomEdgeOffset(float value);
    void setTopEdgeOffset(float value);

    void renderSelectionHighlight(float aspectRatio);
//    void renderEditingHandles(float aspectRatio, float scale);
    
    void renderAsRelation(float aspectRatio) const;
    bool contains(float x, float y, float aspectRatio);
    bool isRelatedTo(LayoutComponent* component) const;
    bool isHorizontalEdge(const LayoutComponentEdge& edge) const;
    bool isPositiveEdge(const LayoutComponentEdge& edge) const;
    float getLeft(float aspectRatio) const;
    float getRight(float aspectRatio) const;
    float getBottom() const;
    float getTop() const;
    Layout& getLayout();
    std::string getName() const;
    std::vector<std::string> getAvailableComponentNames();
    void getProperties(PropertyMaker& owner, const Metadata& metadata);

    /***********************\
     * Scripting Interface *
    \***********************/
    void setScreen(IScreen* screen);
    
    private:
    
    // JSON members.
    static const std::string JSON_BOTTOM;
    static const std::string JSON_LEFT;
    static const std::string JSON_RIGHT;
    static const std::string JSON_SCREEN;
    static const std::string JSON_TOP;

    // External interfaces.
    Layout& cLayout;

    // Definition data.
    Screen cDefScreen;
    LayoutComponentEdge cDefLeftEdge;
    LayoutComponentEdge cDefRightEdge;
    LayoutComponentEdge cDefBottomEdge;
    LayoutComponentEdge cDefTopEdge;
    
    // Runtime data.
    IScreen* cRuntimeScreen;
    
    // Scripting support.
    LuaBinding<LayoutComponent> cLuaBinding;

    // Private functions.
    // void renderEditingHandle(float x, float y, float radius, Handle handle);
    // void testHandlePick(float xPick, float yPick, float x, float y, float radius, LayoutComponent::Handle handle);
  };
}
