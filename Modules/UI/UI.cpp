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
  const std::string UI::SCREEN_GRADIENT = "Gradient";
  const std::string UI::SCREEN_MODEL    = "Model";
  const std::string UI::SCREEN_PANEL    = "Panel";
  const std::string UI::SCREEN_TEXT     = "Text";

  const std::string UI::STRING_TIME = "Time";

  UI::UI(Project& project, IResourceTypeRegistry& registry):
                    cProject(project),
                    cModule(registry),
                    cLayoutLocations(registry),
                    cLayoutOffsets(registry),
                    cMenuItems(registry),
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
    assets.addProvider(&cProviderScreenGradient, SCREEN_GRADIENT, "A Rectangle Gradient");
    assets.addProvider(&cProviderScreenModel,    SCREEN_MODEL,    "A 3D Model");
    assets.addProvider(&cProviderScreenPanel,    SCREEN_PANEL,    "A Rounded Rectangle");
    assets.addProvider(&cProviderScreenText,     SCREEN_TEXT,     "A Text Label or Value");
    assets.addProvider(&cProviderStringTime,     STRING_TIME,     "A Time Value");
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
