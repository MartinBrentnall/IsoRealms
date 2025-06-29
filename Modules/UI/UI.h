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

#include "Assets/Fixed/LayoutLocation/LayoutLocationAbsolute.h"
#include "Assets/Fixed/LayoutLocation/LayoutLocationRelative.h"
#include "Assets/Fixed/LayoutOffset/LayoutOffsetAbsolute.h"
#include "Assets/Fixed/LayoutOffset/LayoutOffsetLinked.h"
#include "Assets/Fixed/MenuItem/MenuItemAction.h"
#include "Assets/Fixed/MenuItem/MenuItemBoolean.h"
#include "Assets/Fixed/MenuItem/MenuItemDigitalInput.h"
#include "Assets/Fixed/MenuItem/MenuItemDisplayResolution.h"
#include "Assets/Fixed/MenuItem/MenuItemFileList.h"
#include "Assets/Fixed/MenuItem/MenuItemSlider.h"
#include "Assets/Fixed/Screen/ScreenGradient.h"
#include "Assets/Fixed/Screen/ScreenModel.h"
#include "Assets/Fixed/Screen/ScreenPanel.h"
#include "Assets/Fixed/Screen/ScreenText.h"
#include "Assets/Fixed/String/StringTime.h"
#include "IUI.h"
#include "Layout/Layout.h"
#include "Menu/Menu.h"
#include "Prompt/Prompt.h"
#include "ScreenFader/ScreenFader.h"
#include "Throbber/Throbber.h"
#include "VirtualKeyboard/VirtualKeyboard.h"

namespace IsoRealms::UI {
  class UI : public IModuleHandle,
             public IUI {
    public:
    UI(IProject& project, IResourceTypeRegistry* registry);

    // Interface access (used by all).
    IUI& getAssetManager() override;
    IProject& getProject() const override;
    
    bool isReadOnly() const override; // TODO: Probably shouldn't be here.
    void setOwner(File* owner) override; // TODO: Probably shouldn't be here.

    /****************************\
     * Implements IModuleHandle *
    \****************************/
    void load(IProject& project, JSONObject object) override;
    void save(JSONObject object, IAssetIdentifier& identifier) override;
    void registerAssets(IAssetRegistry& assets) override;
    std::vector<std::unique_ptr<IProperty>> getProperties() override;
    
    std::vector<std::string> getAllLayoutLocations() override;
    std::vector<std::string> getAllLayoutOffsets() override;
    std::vector<std::string> getAllMenuItems() override;

    std::string getID(const ILayoutLocation* asset) const override;
    std::string getID(const ILayoutOffset*   asset) const override;
    std::string getID(const IMenuItem*       asset) const override;

    bool renderLayoutLocationIcon(const std::string& id) const override;
    bool renderLayoutOffsetIcon(  const std::string& id) const override;
    bool renderMenuItemIcon(      const std::string& id) const override;

    bool isLayoutLocationConfigurable(const std::string& id) const override;
    bool isLayoutOffsetConfigurable(  const std::string& id) const override;
    bool isMenuItemConfigurable(      const std::string& id) const override;

    ILayoutLocation* createLiteralLayoutLocation(IAssetUser<ILayoutLocation>* user, LayoutComponentEdge& owner) override;
    ILayoutOffset*   createLiteralLayoutOffset(  IAssetUser<ILayoutOffset>*   user, LayoutComponentEdge& owner) override;
    IMenuItem*       createLiteralMenuItem(      IAssetUser<IMenuItem>*       user, Menu&                owner) override;

    ILayoutLocation* getLayoutLocation(IAssetUser<ILayoutLocation>* user, JSONObject object, LayoutComponentEdge& owner) override;
    ILayoutOffset*   getLayoutOffset(  IAssetUser<ILayoutOffset>*   user, JSONObject object, LayoutComponentEdge& owner) override;
    IMenuItem*       getMenuItem(      IAssetUser<IMenuItem>*       user, JSONObject object, Menu&                owner) override;

    ILayoutLocation* getLayoutLocation(IAssetUser<ILayoutLocation>* user, const std::string& id, LayoutComponentEdge& owner) override;
    ILayoutOffset*   getLayoutOffset(  IAssetUser<ILayoutOffset>*   user, const std::string& id, LayoutComponentEdge& owner) override;
    IMenuItem*       getMenuItem(      IAssetUser<IMenuItem>*       user, const std::string& id, Menu&                owner) override;

    void release(IAssetUser<ILayoutLocation>* user, ILayoutLocation* asset) override;
    void release(IAssetUser<ILayoutOffset>*   user, ILayoutOffset*   asset) override;
    void release(IAssetUser<IMenuItem>*       user, IMenuItem*       asset) override;

    void save(JSONObject object, ILayoutLocation* asset) const override;
    void save(JSONObject object, ILayoutOffset*   asset) const override;
    void save(JSONObject object, IMenuItem*       asset) const override;

    private:
    static const std::string ID_RESOURCE_LAYOUT;
    static const std::string ID_RESOURCE_MENU;
    static const std::string ID_RESOURCE_PROMPT;
    static const std::string ID_RESOURCE_SCREEN_FADER;
    static const std::string ID_RESOURCE_THROBBER;
    static const std::string ID_RESOURCE_VIRTUAL_KEYBOARD;

    static const std::string MENU_ITEM_ACTION;
    static const std::string MENU_ITEM_BOOLEAN;
    static const std::string MENU_ITEM_DIGITAL_INPUT;
    static const std::string MENU_ITEM_DISPLAY_RESOLUTION;
    static const std::string MENU_ITEM_FILE_LIST;
    static const std::string MENU_ITEM_SLIDER;

    static const std::string LAYOUT_LOCATION_ABSOLUTE;
    static const std::string LAYOUT_LOCATION_RELATIVE;
    
    static const std::string LAYOUT_OFFSET_ABSOLUTE;
    static const std::string LAYOUT_OFFSET_LINKED;

    static const std::string SCREEN_GRADIENT;
    static const std::string SCREEN_MODEL;
    static const std::string SCREEN_PANEL;
    static const std::string SCREEN_TEXT;

    static const std::string STRING_TIME;

    // External interfaces.
    IProject& cProject;
    
    // Asset registries
    AssetClientManager<LayoutComponentEdge, ILayoutLocation> cLayoutLocations;
    AssetClientManager<LayoutComponentEdge, ILayoutOffset>   cLayoutOffsets;
    AssetClientManager<Menu,                IMenuItem>       cMenuItems;

    // Built-in providers for UI asset types.
    AssetInstanced<LayoutComponentEdge, ILayoutLocation, LayoutLocationAbsolute> cProviderLayoutLocationAbsolute;
    AssetInstanced<LayoutComponentEdge, ILayoutLocation, LayoutLocationRelative> cProviderLayoutLocationRelative;

    AssetInstanced<LayoutComponentEdge, ILayoutOffset, LayoutOffsetAbsolute> cProviderLayoutOffsetAbsolute;
    AssetInstanced<LayoutComponentEdge, ILayoutOffset, LayoutOffsetLinked>   cProviderLayoutOffsetLinked;

    AssetInstanced<Menu, IMenuItem, MenuItemAction>            cProviderMenuItemAction;
    AssetInstanced<Menu, IMenuItem, MenuItemBoolean>           cProviderMenuItemBoolean;
    AssetInstanced<Menu, IMenuItem, MenuItemDigitalInput>      cProviderMenuItemDigitalInput;
    AssetInstanced<Menu, IMenuItem, MenuItemDisplayResolution> cProviderMenuItemDisplayResolution;
    AssetInstanced<Menu, IMenuItem, MenuItemFileList>          cProviderMenuItemFileList;
    AssetInstanced<Menu, IMenuItem, MenuItemSlider>            cProviderMenuItemSlider;

    // Built-in providers for ad-hoc screens.
    AssetInstanced<IResourceData, IScreen, ScreenGradient> cProviderScreenGradient;
    AssetInstanced<IResourceData, IScreen, ScreenModel>    cProviderScreenModel;
    AssetInstanced<IResourceData, IScreen, ScreenPanel>    cProviderScreenPanel;
    AssetInstanced<IResourceData, IScreen, ScreenText>     cProviderScreenText;

    AssetInstanced<IResourceData, IString, StringTime> cProviderStringTime;

    ResourceTypeDefinition<UI, Layout>          cResourceTypeLayout;
    ResourceTypeDefinition<UI, Menu>            cResourceTypeMenu;
    ResourceTypeDefinition<UI, Prompt>          cResourceTypePrompt;
    ResourceTypeDefinition<UI, ScreenFader>     cResourceTypeScreenFader;
    ResourceTypeDefinition<UI, Throbber>        cResourceTypeThrobber;
    ResourceTypeDefinition<UI, VirtualKeyboard> cResourceTypeVirtualKeyboard;
  };
}
