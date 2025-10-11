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

#include "UI.h"

namespace IsoRealms::UI {
  const std::string UI::MENU_ITEM_ACTION             = "Action";
  const std::string UI::MENU_ITEM_BOOLEAN            = "Boolean";
  const std::string UI::MENU_ITEM_DIGITAL_INPUT      = "DigitalInput";
  const std::string UI::MENU_ITEM_DISPLAY_RESOLUTION = "DisplayResolution";
  const std::string UI::MENU_ITEM_FILE_LIST          = "FileList";
  const std::string UI::MENU_ITEM_LAUNCHER_LIST      = "LauncherList";
  const std::string UI::MENU_ITEM_SLIDER             = "Slider";

  const std::string UI::LAYOUT_LOCATION_ABSOLUTE = "Absolute";
  const std::string UI::LAYOUT_LOCATION_RELATIVE = "Relative";
  
  const std::string UI::LAYOUT_OFFSET_ABSOLUTE = "Absolute";
  const std::string UI::LAYOUT_OFFSET_LINKED   = "Linked";

  const std::string UI::SCREEN_GRADIENT = "Gradient";
  const std::string UI::SCREEN_MODEL    = "Model";
  const std::string UI::SCREEN_PANEL    = "Panel";
  const std::string UI::SCREEN_TEXT     = "Text";

  const std::string UI::STRING_TIME = "Time";

  UI::UI(Project& project, IResourceTypeRegistry& registry):
                    cProject(project),
                    cModule(registry),
                    cProviderLayoutLocationAbsolute(registry.getAssetMetadata("LayoutLocationAbsolute")),
                    cProviderLayoutLocationRelative(registry.getAssetMetadata("LayoutLocationRelative")),
                    cProviderLayoutOffsetAbsolute(registry.getAssetMetadata("LayoutOffsetAbsolute")),
                    cProviderLayoutOffsetLinked(registry.getAssetMetadata("LayoutOffsetLinked")),
                    cProviderMenuItemAction(registry.getAssetMetadata("MenuItemAction")),
                    cProviderMenuItemBoolean(registry.getAssetMetadata("MenuItemBoolean")),
                    cProviderMenuItemDigitalInput(registry.getAssetMetadata("MenuItemDigitalInput")),
                    cProviderMenuItemDisplayResolution(registry.getAssetMetadata("MenuItemDisplayResolution")),
                    cProviderMenuItemFileList(registry.getAssetMetadata("MenuItemFileList")),
                    cProviderMenuItemLauncherList(registry.getAssetMetadata("MenuItemLauncherList")),
                    cProviderMenuItemSlider(registry.getAssetMetadata("MenuItemSlider")),
                    cProviderScreenGradient(registry.getAssetMetadata("ScreenGradient")),
                    cProviderScreenModel(registry.getAssetMetadata("ScreenModel")),
                    cProviderScreenPanel(registry.getAssetMetadata("ScreenPanel")),
                    cProviderScreenText(registry.getAssetMetadata("ScreenText")),
                    cProviderStringTime(registry.getAssetMetadata("StringTime")),
                    cResourceTypeLayout(*this),
                    cResourceTypeMenu(*this),
                    cResourceTypePrompt(*this),
                    cResourceTypeScreenFader(*this),
                    cResourceTypeThrobber(*this),
                    cResourceTypeVirtualKeyboard(*this) {
    registry.add(&cResourceTypeLayout,          "Layout");
    registry.add(&cResourceTypeMenu,            "Menu");
    registry.add(&cResourceTypePrompt,          "Prompt");
    registry.add(&cResourceTypeScreenFader,     "ScreenFader");
    registry.add(&cResourceTypeThrobber,        "Throbber");
    registry.add(&cResourceTypeVirtualKeyboard, "VirtualKeyboard");

    // Register UI built-in asset providers.
    cLayoutLocations.add(&cProviderLayoutLocationAbsolute, LAYOUT_LOCATION_ABSOLUTE, "UI");
    cLayoutLocations.add(&cProviderLayoutLocationRelative, LAYOUT_LOCATION_RELATIVE, "UI");

    cLayoutOffsets.add(&cProviderLayoutOffsetAbsolute, LAYOUT_OFFSET_ABSOLUTE, "UI");
    cLayoutOffsets.add(&cProviderLayoutOffsetLinked,   LAYOUT_OFFSET_LINKED,   "UI");
    
    cMenuItems.add(&cProviderMenuItemAction,            MENU_ITEM_ACTION,             "UI");
    cMenuItems.add(&cProviderMenuItemBoolean,           MENU_ITEM_BOOLEAN,            "UI");
    cMenuItems.add(&cProviderMenuItemDigitalInput,      MENU_ITEM_DIGITAL_INPUT,      "UI");
    cMenuItems.add(&cProviderMenuItemDisplayResolution, MENU_ITEM_DISPLAY_RESOLUTION, "UI");
    cMenuItems.add(&cProviderMenuItemFileList,          MENU_ITEM_FILE_LIST,          "UI");
    cMenuItems.add(&cProviderMenuItemLauncherList,      MENU_ITEM_LAUNCHER_LIST,      "UI");
    cMenuItems.add(&cProviderMenuItemSlider,            MENU_ITEM_SLIDER,             "UI");
  }

  const Metadata& UI::getMetadata(const std::string& key) const {
    return cModule.getAssetMetadata(key);
  }

  UI& UI::getAssetManager() {
    return *this;
  }

  Project& UI::getProject() const {
    return cProject;
  }

  bool UI::isReadOnly() const {
    return false;
  } // TODO: Probably shouldn't be here.

  void UI::setOwner(ProjectFile* owner) {
  } // TODO: Probably shouldn't be here.

  void UI::registerAssets(ResourceAssetRegistry& assets) {
    assets.addProvider(&cProviderScreenGradient, SCREEN_GRADIENT, "");
    assets.addProvider(&cProviderScreenModel,    SCREEN_MODEL,    "");
    assets.addProvider(&cProviderScreenPanel,    SCREEN_PANEL,    "");
    assets.addProvider(&cProviderScreenText,     SCREEN_TEXT,     "");
    assets.addProvider(&cProviderStringTime,     STRING_TIME,     "");
  }
  
  void UI::updateInputs(unsigned int milliseconds) {
    // Nothing to do.
  }
  
  void UI::updateRuntime(unsigned int milliseconds) {
    updateRuntime2(cResourceTypeMenu,            milliseconds);
    updateRuntime2(cResourceTypeThrobber,        milliseconds);
    updateRuntime2(cResourceTypeVirtualKeyboard, milliseconds);
    for (std::unique_ptr<ScreenModel>& mScreenModel : cProviderScreenModel) {
      mScreenModel->updateRuntime(milliseconds);
    }
  }
  
  void UI::updateEditing(unsigned int milliseconds) {
    updateEditing2(cResourceTypeLayout, milliseconds);
    for (std::unique_ptr<ScreenModel>& mScreenModel : cProviderScreenModel) {
      mScreenModel->updateEditing(milliseconds);
    }
  }
  
  void UI::reset() {
    reset2(cResourceTypeLayout);
    reset2(cResourceTypeMenu);
    reset2(cResourceTypePrompt);
    reset2(cResourceTypeVirtualKeyboard);
  }  
  
  std::mutex cModuleInstantiationMutex;
  std::vector<std::unique_ptr<UI>> ModuleInstances;
}
 
#ifdef __linux__
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::Project* project, IsoRealms::IResourceTypeRegistry* registry) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::Project * project, IsoRealms::IResourceTypeRegistry * registry) {
#endif
  std::unique_ptr<IsoRealms::UI::UI> mModule = std::make_unique<IsoRealms::UI::UI>(*project, *registry);
  {
    std::lock_guard<std::mutex> mLockGuard(IsoRealms::UI::cModuleInstantiationMutex);
    return IsoRealms::UI::ModuleInstances.emplace_back(std::move(mModule)).get();
  }
}

#ifdef __linux__
extern "C" void destroy(IsoRealms::IModuleHandle* module) {
#elif _WIN32
extern "C" void __declspec(dllexport) __stdcall destroy(IsoRealms::IModuleHandle* module) {
#endif
  std::unique_ptr<IsoRealms::UI::UI> mModule;
  {
    std::lock_guard<std::mutex> mLockGuard(IsoRealms::UI::cModuleInstantiationMutex);
    mModule = IsoRealms::Utils::removeElementUnique(IsoRealms::UI::ModuleInstances, module);
  }
}
