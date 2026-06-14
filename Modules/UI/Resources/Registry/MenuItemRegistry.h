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

#include "IsoRealms.h"

#include "Modules/UI/Resources/Fixed/MenuItem/MenuItemAction.h"
#include "Modules/UI/Resources/Fixed/MenuItem/MenuItemBoolean.h"
#include "Modules/UI/Resources/Fixed/MenuItem/MenuItemDigitalInput.h"
#include "Modules/UI/Resources/Fixed/MenuItem/MenuItemDisplayResolution.h"
#include "Modules/UI/Resources/Fixed/MenuItem/MenuItemFileList.h"
#include "Modules/UI/Resources/Fixed/MenuItem/MenuItemLauncherList.h"
#include "Modules/UI/Resources/Fixed/MenuItem/MenuItemSlider.h"
#include "Modules/UI/Resources/Type/IMenuItem.h"
#include "Modules/UI/Menu/Menu.h"

namespace IsoRealms::UI {
  class Menu;

  class MenuItemRegistry : public ResourceClientManager<MenuItemRegistry, Menu, IMenuItem> {
    public:
    MenuItemRegistry(IComponentTypeRegistry& registry);

    private:
    ResourceInstanced<Menu, IMenuItem, MenuItemAction>            cAction;
    ResourceInstanced<Menu, IMenuItem, MenuItemBoolean>           cBoolean;
    ResourceInstanced<Menu, IMenuItem, MenuItemDigitalInput>      cDigitalInput;
    ResourceInstanced<Menu, IMenuItem, MenuItemDisplayResolution> cDisplayResolution;
    ResourceInstanced<Menu, IMenuItem, MenuItemFileList>          cFileList;
    ResourceInstanced<Menu, IMenuItem, MenuItemLauncherList>      cLauncherList;
    ResourceInstanced<Menu, IMenuItem, MenuItemSlider>            cSlider;
  };
}
