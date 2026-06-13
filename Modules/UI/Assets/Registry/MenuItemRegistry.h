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

#include "Modules/UI/Assets/Fixed/MenuItem/MenuItemAction.h"
#include "Modules/UI/Assets/Fixed/MenuItem/MenuItemBoolean.h"
#include "Modules/UI/Assets/Fixed/MenuItem/MenuItemDigitalInput.h"
#include "Modules/UI/Assets/Fixed/MenuItem/MenuItemDisplayResolution.h"
#include "Modules/UI/Assets/Fixed/MenuItem/MenuItemFileList.h"
#include "Modules/UI/Assets/Fixed/MenuItem/MenuItemLauncherList.h"
#include "Modules/UI/Assets/Fixed/MenuItem/MenuItemSlider.h"
#include "Modules/UI/Assets/Type/IMenuItem.h"
#include "Modules/UI/Menu/Menu.h"

namespace IsoRealms::UI {
  class Menu;

  class MenuItemRegistry : public AssetClientManager<MenuItemRegistry, Menu, IMenuItem> {
    public:
    MenuItemRegistry(IComponentTypeRegistry& registry);

    private:
    AssetInstanced<Menu, IMenuItem, MenuItemAction>            cAction;
    AssetInstanced<Menu, IMenuItem, MenuItemBoolean>           cBoolean;
    AssetInstanced<Menu, IMenuItem, MenuItemDigitalInput>      cDigitalInput;
    AssetInstanced<Menu, IMenuItem, MenuItemDisplayResolution> cDisplayResolution;
    AssetInstanced<Menu, IMenuItem, MenuItemFileList>          cFileList;
    AssetInstanced<Menu, IMenuItem, MenuItemLauncherList>      cLauncherList;
    AssetInstanced<Menu, IMenuItem, MenuItemSlider>            cSlider;
  };
}
