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

#include <iostream>
#include <mutex>
#include <stack>
#include <string>
#include <vector>

#include "IsoRealms/Assets/Providers/AssetInstanced.h"
#include "IsoRealms/IModuleHandle.h"
#include "IsoRealms/IsoRealmsConstants.h"
#include "IsoRealms/Project.h"
#include "IsoRealms/ResourceTypeDefinition.h"
#include "IsoRealms/System.h"

#include "Assets/Fixed/MenuItem/MenuItemAction.h"
#include "Assets/Fixed/MenuItem/MenuItemBoolean.h"
#include "Assets/Fixed/MenuItem/MenuItemDigitalInput.h"
#include "Assets/Fixed/MenuItem/MenuItemDisplayResolution.h"
#include "Assets/Fixed/MenuItem/MenuItemDummy.h"
#include "Assets/Fixed/MenuItem/MenuItemFileList.h"
#include "Assets/Fixed/MenuItem/MenuItemSlider.h"
#include "Layout/Layout.h"
#include "Menu/Menu.h"
#include "Panel/Panel.h"
#include "Prompt/Prompt.h"
#include "Throbber/Throbber.h"
#include "VirtualKeyboard/VirtualKeyboard.h"

namespace IsoRealms::UI {
  class UI : public IModuleHandle {
    public:
    UI(IProject* project, IResourceTypeRegistry* registry, IAssetLiterals* literals);

    // Interface access (used by all).
    IProject* getProject() const;

    /****************************\
     * Implements IModuleHandle *
    \****************************/
    void load(IProject* project, JSONObject object) override;
    void save(JSONObject object, IAssetIdentifier* identifier) override;
    void registerAssets(IAssetRegistry* assets) override;
    void unregisterAssets(IAssetRemover* remover, IAssets* releaser) override;
    
    IMenuItem* createLiteralMenuItem(IAssetUser<IMenuItem>* user);

    IMenuItem* getMenuItem(IAssetUser<IMenuItem>* user, JSONObject object, Menu* owner);

    void release(IAssetUser<IMenuItem>* user, IMenuItem* asset);

    void save(JSONObject object, IMenuItem* asset) const;

    private:
    static const std::string ID_RESOURCE_LAYOUT;
    static const std::string ID_RESOURCE_MENU;
    static const std::string ID_RESOURCE_PANEL;
    static const std::string ID_RESOURCE_PROMPT;
    static const std::string ID_RESOURCE_THROBBER;
    static const std::string ID_RESOURCE_VIRTUAL_KEYBOARD;

    static const std::string NAME_RESOURCE_LAYOUT;
    static const std::string NAME_RESOURCE_MENU;
    static const std::string NAME_RESOURCE_PANEL;
    static const std::string NAME_RESOURCE_PROMPT;
    static const std::string NAME_RESOURCE_THROBBER;
    static const std::string NAME_RESOURCE_VIRTUAL_KEYBOARD;

    static const std::string MENU_ITEM_ACTION;
    static const std::string MENU_ITEM_BOOLEAN;
    static const std::string MENU_ITEM_DIGITAL_INPUT;
    static const std::string MENU_ITEM_DISPLAY_RESOLUTION;
    static const std::string MENU_ITEM_FILE_LIST;
    static const std::string MENU_ITEM_SLIDER;

    IProject* cProject;

    AssetClientManager<Menu, IMenuItem> cMenuItems;

    // Dummy asset providers.
    AssetLiteralDummy<Menu, IMenuItem, MenuItemDummy> cDummyProviderMenuItem;

    // Built-in providers for Spindizzy asset types.
    AssetInstanced<Menu, IMenuItem, MenuItemAction>            cProviderMenuItemAction;
    AssetInstanced<Menu, IMenuItem, MenuItemBoolean>           cProviderMenuItemBoolean;
    AssetInstanced<Menu, IMenuItem, MenuItemDigitalInput>      cProviderMenuItemDigitalInput;
    AssetInstanced<Menu, IMenuItem, MenuItemDisplayResolution> cProviderMenuItemDisplayResolution;
    AssetInstanced<Menu, IMenuItem, MenuItemFileList>          cProviderMenuItemFileList;
    AssetInstanced<Menu, IMenuItem, MenuItemSlider>            cProviderMenuItemSlider;

    ResourceTypeDefinition<UI, Layout>          cResourceTypeLayout;
    ResourceTypeDefinition<UI, Menu>            cResourceTypeMenu;
    ResourceTypeDefinition<UI, Panel>           cResourceTypePanel;
    ResourceTypeDefinition<UI, Prompt>          cResourceTypePrompt;
    ResourceTypeDefinition<UI, Throbber>        cResourceTypeThrobber;
    ResourceTypeDefinition<UI, VirtualKeyboard> cResourceTypeVirtualKeyboard;
  };
}
