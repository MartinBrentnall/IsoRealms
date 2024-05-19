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

#include "UI.h"

namespace IsoRealms::UI {
  const std::string UI::ID_RESOURCE_LAYOUT           = "Layout";
  const std::string UI::ID_RESOURCE_MENU             = "Menu";
  const std::string UI::ID_RESOURCE_PANEL            = "Panel";
  const std::string UI::ID_RESOURCE_PROMPT           = "Prompt";
  const std::string UI::ID_RESOURCE_THROBBER         = "Throbber";
  const std::string UI::ID_RESOURCE_VIRTUAL_KEYBOARD = "VirtualKeyboard";

  const std::string UI::NAME_RESOURCE_LAYOUT           = "Layouts";
  const std::string UI::NAME_RESOURCE_MENU             = "Menus";
  const std::string UI::NAME_RESOURCE_PANEL            = "Panels";
  const std::string UI::NAME_RESOURCE_PROMPT           = "Prompts";
  const std::string UI::NAME_RESOURCE_THROBBER         = "Throbbers";
  const std::string UI::NAME_RESOURCE_VIRTUAL_KEYBOARD = "Virtual Keyboards";

  UI::UI(IProject* project, IResourceTypeRegistry* registry, IAssetLiterals* literals):
                    cResourceTypeLayout(this),
                    cResourceTypeMenu(this),
                    cResourceTypePanel(this),
                    cResourceTypePrompt(this),
                    cResourceTypeThrobber(this),
                    cResourceTypeVirtualKeyboard(this) {
    registry->add(&cResourceTypeLayout,          ID_RESOURCE_LAYOUT,           NAME_RESOURCE_LAYOUT,           IsoRealmsConstants::RESOURCE_CATEGORY_PRESENTATION);
    registry->add(&cResourceTypeMenu,            ID_RESOURCE_MENU,             NAME_RESOURCE_MENU,             IsoRealmsConstants::RESOURCE_CATEGORY_PRESENTATION);
    registry->add(&cResourceTypePanel,           ID_RESOURCE_PANEL,            NAME_RESOURCE_PANEL,            IsoRealmsConstants::RESOURCE_CATEGORY_PRESENTATION);
    registry->add(&cResourceTypePrompt,          ID_RESOURCE_PROMPT,           NAME_RESOURCE_PROMPT,           IsoRealmsConstants::RESOURCE_CATEGORY_PRESENTATION);
    registry->add(&cResourceTypeThrobber,        ID_RESOURCE_THROBBER,         NAME_RESOURCE_THROBBER,         IsoRealmsConstants::RESOURCE_CATEGORY_PRESENTATION);
    registry->add(&cResourceTypeVirtualKeyboard, ID_RESOURCE_VIRTUAL_KEYBOARD, NAME_RESOURCE_VIRTUAL_KEYBOARD, IsoRealmsConstants::RESOURCE_CATEGORY_PRESENTATION);
  }

  void UI::load(IProject* project, JSONObject object) {
    // Nothing to do.
  }

  void UI::save(JSONObject object, IAssetIdentifier* identifier) {
    // Nothing to do.
  }

  void UI::registerAssets(IAssetRegistry* assets) {
    // Nothing to do.
  }
  
  void UI::unregisterAssets(IAssetRemover* remover, IAssets* releaser) {
    // Nothing to do.
  }

  std::mutex cModuleInstantiationMutex;
  std::vector<std::unique_ptr<UI>> ModuleInstances;
}
 
#ifdef __linux__
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::IProject* project, IsoRealms::IResourceTypeRegistry* registry, IsoRealms::IAssetLiterals* literals) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::IProject * project, IsoRealms::IResourceTypeRegistry * registry, IsoRealms::IAssetLiterals * literals) {
#endif
  std::lock_guard<std::mutex> mLockGuard(IsoRealms::UI::cModuleInstantiationMutex);
  return IsoRealms::UI::ModuleInstances.emplace_back(std::make_unique<IsoRealms::UI::UI>(project, registry, literals)).get();
}

#ifdef __linux__
extern "C" void destroy(IsoRealms::IModuleHandle* module) {
#elif _WIN32
extern "C" void __declspec(dllexport) __stdcall destroy(IsoRealms::IModuleHandle* module) {
#endif
  std::lock_guard<std::mutex> mLockGuard(IsoRealms::UI::cModuleInstantiationMutex);
  IsoRealms::Utils::removeElementUnique(IsoRealms::UI::ModuleInstances, module);
}
