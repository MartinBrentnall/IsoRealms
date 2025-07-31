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

#include <iostream>
#include <mutex>
#include <stack>
#include <string>
#include <vector>

#include "IsoRealms.h"

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
#include "Layout/Layout.h"
#include "Menu/Menu.h"
#include "Prompt/Prompt.h"
#include "ScreenFader/ScreenFader.h"
#include "Throbber/Throbber.h"
#include "VirtualKeyboard/VirtualKeyboard.h"

namespace IsoRealms::UI {
  template<class TYPE> struct AssetContainerTraits;

  class UI : public IModuleHandle {
    public:
    UI(Project& project, IResourceTypeRegistry& registry);

    // Interface access (used by all).
    const Metadata& getMetadata(const std::string& key) const;
    UI& getAssetManager();
    Project& getProject() const;
    
    bool isReadOnly() const; // TODO: Probably shouldn't be here.
    void setOwner(ProjectFile* owner); // TODO: Probably shouldn't be here.

    /****************************\
     * Implements IModuleHandle *
    \****************************/
    void load(IProject& project, JSONObject object) override;
    void save(JSONObject object) override;
    void registerAssets(ResourceAssetRegistry& assets) override;
    void getProperties() override;
    void updateRuntime(unsigned int milliseconds) override;
    void updateEditing(unsigned int milliseconds) override;
    void reset() override;
    
    ILayoutLocation* createLiteralLayoutLocation(IAssetUser<ILayoutLocation>* user, LayoutComponentEdge& owner);
    ILayoutOffset*   createLiteralLayoutOffset(  IAssetUser<ILayoutOffset>*   user, LayoutComponentEdge& owner);
    IMenuItem*       createLiteralMenuItem(      IAssetUser<IMenuItem>*       user, Menu&                owner);

    ILayoutLocation* getLayoutLocation(IAssetUser<ILayoutLocation>* user, JSONObject object, LayoutComponentEdge& owner);
    ILayoutOffset*   getLayoutOffset(  IAssetUser<ILayoutOffset>*   user, JSONObject object, LayoutComponentEdge& owner);
    IMenuItem*       getMenuItem(      IAssetUser<IMenuItem>*       user, JSONObject object, Menu&                owner);

    ILayoutLocation* getLayoutLocation(IAssetUser<ILayoutLocation>* user, const std::string& id, LayoutComponentEdge& owner);
    ILayoutOffset*   getLayoutOffset(  IAssetUser<ILayoutOffset>*   user, const std::string& id, LayoutComponentEdge& owner);
    IMenuItem*       getMenuItem(      IAssetUser<IMenuItem>*       user, const std::string& id, Menu&                owner);

    template <typename TYPE> void release(IAssetUser<TYPE>* user, TYPE* asset) {
      AssetContainerTraits<TYPE>::get(*this).release(user, asset);
    }

    template <typename TYPE> std::string getID(const TYPE* asset) const {
      return AssetContainerTraits<TYPE>::get(*this).getID(asset);
    }

    template <typename TYPE> void save(JSONObject object, const TYPE* asset) const {
      AssetContainerTraits<TYPE>::get(*this).save(object, asset);
    }

    template <typename TYPE> std::vector<std::string> getAll() const {
      return AssetContainerTraits<TYPE>::get(*this).getAll();
    }

    template <typename TYPE> bool renderIcon(const std::string& id) const {
      return AssetContainerTraits<TYPE>::get(*this).renderIcon(id);
    }

    template <typename TYPE> bool isConfigurable(const std::string& id) const {
      return AssetContainerTraits<TYPE>::get(*this).hasConfiguration(id);
    }

    private:
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
    Project& cProject;
    IResourceTypeRegistry& cModule;

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

    template <class TYPE> friend struct AssetContainerTraits;
  };

  template<> struct AssetContainerTraits<ILayoutLocation> {template<class UI> static auto& get(UI& ui) {return ui.cLayoutLocations;}};
  template<> struct AssetContainerTraits<ILayoutOffset>   {template<class UI> static auto& get(UI& ui) {return ui.cLayoutOffsets;  }};
  template<> struct AssetContainerTraits<IMenuItem>       {template<class UI> static auto& get(UI& ui) {return ui.cMenuItems;      }};
}
