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

#include "IsoRealms/IModuleHandle.h"
#include "IsoRealms/IsoRealmsConstants.h"
#include "IsoRealms/Project.h"
#include "IsoRealms/ResourceTypeDefinition.h"
#include "IsoRealms/System.h"

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

    /****************************\
     * Implements IModuleHandle *
    \****************************/
    void load(IProject* project, JSONObject object) override;
    void save(JSONObject object, IAssetIdentifier* identifier) override;
    void registerAssets(IAssetRegistry* assets) override;
    void unregisterAssets(IAssetRemover* remover, IAssets* releaser) override;
    
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

    ResourceTypeDefinition<UI, Layout>          cResourceTypeLayout;
    ResourceTypeDefinition<UI, Menu>            cResourceTypeMenu;
    ResourceTypeDefinition<UI, Panel>           cResourceTypePanel;
    ResourceTypeDefinition<UI, Prompt>          cResourceTypePrompt;
    ResourceTypeDefinition<UI, Throbber>        cResourceTypeThrobber;
    ResourceTypeDefinition<UI, VirtualKeyboard> cResourceTypeVirtualKeyboard;
  };
}
