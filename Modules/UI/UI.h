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

#include "Resources/Registry/LayoutLocationRegistry.h"
#include "Resources/Registry/LayoutOffsetRegistry.h"
#include "Resources/Registry/MenuItemRegistry.h"
#include "Resources/Fixed/Screen/ScreenGradient.h"
#include "Resources/Fixed/Screen/ScreenModel.h"
#include "Resources/Fixed/Screen/ScreenPanel.h"
#include "Resources/Fixed/Screen/ScreenText.h"
#include "Resources/Fixed/String/StringTime.h"
#include "Layout/Layout.h"
#include "Menu/Menu.h"
#include "Prompt/Prompt.h"
#include "ScreenFader/ScreenFader.h"
#include "Throbber/Throbber.h"
#include "VirtualKeyboard/VirtualKeyboard.h"

namespace IsoRealms::UI {
  template <typename TYPE> struct ResourceContainerTraits;

  class UI : public IModuleHandle {
    public:
    UI(Project& project, IComponentTypeRegistry& registry);

    // Interface access (used by all).
    const Metadata& getMetadata(const std::string& key) const;
    UI& getResourceManager();
    Project& getProject() const;
    
    bool isReadOnly() const; // TODO: Probably shouldn't be here.
    void setOwner(ProjectFile* owner); // TODO: Probably shouldn't be here.

    /****************************\
     * Implements IModuleHandle *
    \****************************/
    void publish(ResourcePublisher& publisher) override;
    void updateInputs(unsigned int milliseconds) override;
    void updateRuntime(unsigned int milliseconds) override;
    void updateEditing(unsigned int milliseconds) override;
    void reset() override;
    
    template <typename TYPE> void release(IResourceUser<TYPE>* user, TYPE* resource) {
      ResourceContainerTraits<TYPE>::get(*this).release(user, resource);
    }

    template <typename TYPE> TreeItemInfo getTreeItemInfo(const TYPE* resource) const {
      return ResourceContainerTraits<TYPE>::get(*this).getTreeItemInfo(resource);
    }

    template <typename TYPE> const Metadata& getPropertyMetadata(const TYPE* resource) const {
      return ResourceContainerTraits<TYPE>::get(*this).getPropertyMetadata(resource);
    }

    template <typename TYPE> void save(JSONObject object, const TYPE* resource) const {
      ResourceContainerTraits<TYPE>::get(*this).save(object, resource);
    }

    template <typename TYPE> void forEachEntry(const std::function<void(const TreeItemInfo&)>& treeItemInfoFunction) const {
      ResourceContainerTraits<TYPE>::get(*this).forEachEntry(treeItemInfoFunction);
    }

    template <typename TYPE> bool renderIcon(const std::string& id) const {
      return ResourceContainerTraits<TYPE>::get(*this).renderIcon(id);
    }

    template <typename TYPE> bool isConfigurable(const std::string& id) const {
      return ResourceContainerTraits<TYPE>::get(*this).hasConfiguration(id);
    }

    template <typename TYPE, typename OWNER> TYPE* createDefault(IResourceUser<TYPE>* user, OWNER& owner) {
      return ResourceContainerTraits<TYPE>::get(*this).getDefault(user, owner);
    }

    template <typename TYPE, typename OWNER> TYPE* getResource(IResourceUser<TYPE>* user, const std::string& id, OWNER& owner, IStateListener* listener = nullptr) {
      return ResourceContainerTraits<TYPE>::get(*this).get(user, owner, id, listener);
    }

    template <typename TYPE, typename OWNER> TYPE* getResource(IResourceUser<TYPE>* user, JSONObject object, OWNER& owner, IStateListener* listener = nullptr, bool required = true) {
      return ResourceContainerTraits<TYPE>::get(*this).get(user, owner, object, listener, required);
    }

    private:
    inline static const std::string SCREEN_GRADIENT = "Gradient";
    inline static const std::string SCREEN_MODEL    = "Model";
    inline static const std::string SCREEN_PANEL    = "Panel";
    inline static const std::string SCREEN_TEXT     = "Text";

    inline static const std::string STRING_TIME = "Time";

    // External interfaces.
    Project& cProject;
    IComponentTypeRegistry& cModule;

    // Resource registries
    LayoutLocationRegistry cLayoutLocations;
    LayoutOffsetRegistry   cLayoutOffsets;
    MenuItemRegistry       cMenuItems;

    // Built-in providers for ad-hoc screens.
    ResourceInstanced<IComponentData, IScreen, ScreenGradient> cProviderScreenGradient;
    ResourceInstanced<IComponentData, IScreen, ScreenModel>    cProviderScreenModel;
    ResourceInstanced<IComponentData, IScreen, ScreenPanel>    cProviderScreenPanel;
    ResourceInstanced<IComponentData, IScreen, ScreenText>     cProviderScreenText;

    ResourceInstanced<IComponentData, IString, StringTime> cProviderStringTime;

    ComponentTypeDefinition<UI, Layout>          cComponentTypeLayout;
    ComponentTypeDefinition<UI, Menu>            cComponentTypeMenu;
    ComponentTypeDefinition<UI, Prompt>          cComponentTypePrompt;
    ComponentTypeDefinition<UI, ScreenFader>     cComponentTypeScreenFader;
    ComponentTypeDefinition<UI, Throbber>        cComponentTypeThrobber;
    ComponentTypeDefinition<UI, VirtualKeyboard> cComponentTypeVirtualKeyboard;

    template <class TYPE> friend struct ResourceContainerTraits;
  };

  template<> struct ResourceContainerTraits<ILayoutLocation> {template <typename UI> static auto& get(UI& ui) {return ui.cLayoutLocations;}};
  template<> struct ResourceContainerTraits<ILayoutOffset>   {template <typename UI> static auto& get(UI& ui) {return ui.cLayoutOffsets;  }};
  template<> struct ResourceContainerTraits<IMenuItem>       {template <typename UI> static auto& get(UI& ui) {return ui.cMenuItems;      }};
}
