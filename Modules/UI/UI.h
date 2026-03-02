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

#include "Assets/Registry/LayoutLocationRegistry.h"
#include "Assets/Registry/LayoutOffsetRegistry.h"
#include "Assets/Registry/MenuItemRegistry.h"
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
  template <typename TYPE> struct AssetContainerTraits;

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
    void registerAssets(ResourceAssetRegistry& assets) override;
    void updateInputs(unsigned int milliseconds) override;
    void updateRuntime(unsigned int milliseconds) override;
    void updateEditing(unsigned int milliseconds) override;
    void reset() override;
    
    template <typename TYPE> void release(IAssetUser<TYPE>* user, TYPE* asset) {
      AssetContainerTraits<TYPE>::get(*this).release(user, asset);
    }

    template <typename TYPE> AssetInfo getAssetInfo(const TYPE* asset) const {
      return AssetContainerTraits<TYPE>::get(*this).getAssetInfo(asset);
    }

    template <typename TYPE> void save(JSONObject object, const TYPE* asset) const {
      AssetContainerTraits<TYPE>::get(*this).save(object, asset);
    }

    template <typename TYPE> void forEachEntry(std::function<void(const AssetInfo&)> assetInfoFunction) const {
      AssetContainerTraits<TYPE>::get(*this).forEachEntry(assetInfoFunction);
    }

    template <typename TYPE> bool renderIcon(const std::string& id) const {
      return AssetContainerTraits<TYPE>::get(*this).renderIcon(id);
    }

    template <typename TYPE> bool isConfigurable(const std::string& id) const {
      return AssetContainerTraits<TYPE>::get(*this).hasConfiguration(id);
    }

    template <typename TYPE, typename OWNER> TYPE* createDefault(IAssetUser<TYPE>* user, OWNER& owner) {
      return AssetContainerTraits<TYPE>::get(*this).getDefault(user, owner);
    }

    template <typename TYPE, typename OWNER> TYPE* getAsset(IAssetUser<TYPE>* user, const std::string& id, OWNER& owner, IStateListener* listener = nullptr) {
      return AssetContainerTraits<TYPE>::get(*this).get(user, owner, id, listener);
    }

    template <typename TYPE, typename OWNER> TYPE* getAsset(IAssetUser<TYPE>* user, JSONObject object, OWNER& owner, IStateListener* listener = nullptr, bool required = true) {
      return AssetContainerTraits<TYPE>::get(*this).get(user, owner, object, listener, required);
    }

    private:
    static const std::string SCREEN_GRADIENT;
    static const std::string SCREEN_MODEL;
    static const std::string SCREEN_PANEL;
    static const std::string SCREEN_TEXT;

    static const std::string STRING_TIME;

    // External interfaces.
    Project& cProject;
    IResourceTypeRegistry& cModule;

    // Asset registries
    LayoutLocationRegistry cLayoutLocations;
    LayoutOffsetRegistry   cLayoutOffsets;
    MenuItemRegistry       cMenuItems;

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

  template<> struct AssetContainerTraits<ILayoutLocation> {template <typename UI> static auto& get(UI& ui) {return ui.cLayoutLocations;}};
  template<> struct AssetContainerTraits<ILayoutOffset>   {template <typename UI> static auto& get(UI& ui) {return ui.cLayoutOffsets;  }};
  template<> struct AssetContainerTraits<IMenuItem>       {template <typename UI> static auto& get(UI& ui) {return ui.cMenuItems;      }};
}
