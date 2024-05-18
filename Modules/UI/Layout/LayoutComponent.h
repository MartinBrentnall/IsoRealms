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

#include <map>
#include <string>

#include "IsoRealms/Collision/CollisionUtils.h"
#include "IsoRealms/IAssetBrowser.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

#include "Offset/AbsoluteOffset.h"
#include "Offset/LinkedOffset.h"
#include "Location/AbsoluteLocation.h"
#include "Location/RelativeLocation.h"

namespace IsoRealms::UI {
  class Layout;

  class LayoutComponent final {
    public:
    LayoutComponent(IProject* project, Layout& layout);
    LayoutComponent(IProject* project, Layout& layout, JSONObject object);
    void registerAssets(IAssetRegistry* assets, const std::string& name);
    void unregisterAssets(IAssetRemover* assets);
    void render(float scale, float aspectRatio);
    void save(JSONObject object, IAssetIdentifier* identifier) const;
    bool inputEditor(sf::Event& event, IScreen* screen, float x, float y, float aspectRatio, float scale);
    bool renderSelectionHighlight(float aspectRatio, float scale);
    bool contains(float x, float y, float aspectRatio);
    bool isRelatedTo(LayoutComponent* component) const;
    float getLeft(float aspectRatio) const;
    float getRight(float aspectRatio) const;
    float getBottom() const;
    float getTop() const;

    /***********************\
     * Scripting Interface *
    \***********************/
    void setScreen(IScreen* screen);
    
    private:
    
    // JSON members.
    static const std::string JSON_BOTTOM;
    static const std::string JSON_LEFT;
    static const std::string JSON_LOCATION;
    static const std::string JSON_OFFSET;
    static const std::string JSON_RIGHT;
    static const std::string JSON_SCREEN;
    static const std::string JSON_TOP;
    static const std::string JSON_TYPE;

    static const std::string LOCATION_TYPE_ABSOLUTE;
    static const std::string LOCATION_TYPE_RELATIVE;

    static const std::string OFFSET_TYPE_ABSOLUTE;
    static const std::string OFFSET_TYPE_LINKED;

    // Private types.
    class Edge {
      public:
      LayoutComponent& cDefParent;
      std::unique_ptr<ILayoutLocation> cDefLocation;
      std::unique_ptr<ILayoutOffset> cDefOffset;
      
      Edge(LayoutComponent& parent, float location);
      Edge(IProject* project, LayoutComponent& parent, JSONObject object, const std::string& tag, bool horizontal, float defaultValue);

      float getLocation(float aspectRatio) const;
      void save(JSONObject object, const std::string& tag, Layout* layout, float defaultValue) const;
    };
    
    enum class Handle {
      NONE,
      WEST,
      EAST,
      SOUTH,
      NORTH,
      SOUTHWEST,
      SOUTHEAST,
      NORTHWEST,
      NORTHEAST
    };
      
    static const float EDIT_HANDLE_RADIUS;
    
    // Definition data.
    Layout& cDefLayout;
    Screen cDefScreen;
    Edge cDefLeftEdge;
    Edge cDefRightEdge;
    Edge cDefBottomEdge;
    Edge cDefTopEdge;
    
    // Runtime data.
    IScreen* cRuntimeScreen;
    
    // Editing data.
    Handle cEditingSelectedHandle;
    bool cEditingDragging;
    
    // Scripting support.
    LuaBinding<LayoutComponent> cLuaBinding;

    // Private functions.
    void renderEditingHandle(float, float, float, Handle);
    void testHandlePick(float xPick, float yPick, float x, float y, float radius, LayoutComponent::Handle handle);
  };
}
