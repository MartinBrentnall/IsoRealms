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

#include <stack>
#include <vector>

#include "IsoRealms/IModule.h"
#include "IsoRealms/Input/HatHandler.h"
#include "IsoRealms/Options/LocalOptions.h"
#include "IsoRealms/Project.h"
#include "IsoRealms/ResourceDefinition.h"

#include "ConfiguratorSignalID.h"
#include "IConfiguratorScreen.h"
#include "IUIConfiguration.h"

namespace IsoRealms {
  class Configurator final {
    public:
    Configurator(IUIConfiguration* editor);

    void update(unsigned int milliseconds);
    void open(IConfiguratorScreen* screen);
    void input(ConfiguratorSignalID id);

    const IColour* getSelectionHighlight() const;
    const IColour* getSelectionLocked() const;
    IFont* getFont() const;
    float getScreenLeftBorder(float aspectRatio) const;
    float getScreenTopBorder() const;
    float getTopIconPosition() const;
//    float getIconHeight() const;
    float getLeftIconPosition(float aspectRatio) const;
    float getMenuBoundaryBottom() const;
    float getMenuBoundaryTop() const;
//    float getExtraActionSpacing() const;
    
    void closeProjectMenu();

    void render(float aspectRatio) const;

    private:
    static const LiteralColour COLOUR_TRANSPARENT;       /// Transparency
    static const float MENU_BOUNDARY_BOTTOM;
    static const float MENU_BOUNDARY_TOP;
    static const float SCREEN_BORDER_SIZE;

    IUIConfiguration* cEditor;

    std::vector<IConfiguratorScreen*> cRuntimeMenus;/// Stack of project menus.
    std::vector<IConfiguratorScreen*> cRuntimeClosedMenus; /// Closed project menus that are still being animated.
  };
}
