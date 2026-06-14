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
  UI::UI(Project& project, IComponentTypeRegistry& registry):
                    cProject(project),
                    cModule(registry),
                    cLayoutLocations(registry),
                    cLayoutOffsets(registry),
                    cMenuItems(registry),
                    cProviderScreenGradient(registry.getResourceMetadata("ScreenGradient")),
                    cProviderScreenModel(registry.getResourceMetadata("ScreenModel")),
                    cProviderScreenPanel(registry.getResourceMetadata("ScreenPanel")),
                    cProviderScreenText(registry.getResourceMetadata("ScreenText")),
                    cProviderStringTime(registry.getResourceMetadata("StringTime")),
                    cComponentTypeLayout(*this),
                    cComponentTypeMenu(*this),
                    cComponentTypePrompt(*this),
                    cComponentTypeScreenFader(*this),
                    cComponentTypeThrobber(*this),
                    cComponentTypeVirtualKeyboard(*this) {
    registry.add(&cComponentTypeLayout,          "Layout");
    registry.add(&cComponentTypeMenu,            "Menu");
    registry.add(&cComponentTypePrompt,          "Prompt");
    registry.add(&cComponentTypeScreenFader,     "ScreenFader");
    registry.add(&cComponentTypeThrobber,        "Throbber");
    registry.add(&cComponentTypeVirtualKeyboard, "VirtualKeyboard");
  }

  const Metadata& UI::getMetadata(const std::string& key) const {
    return cModule.getResourceMetadata(key);
  }

  UI& UI::getResourceManager() {
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

  void UI::publish(ResourcePublisher& publisher) {
    publisher.publishProvider(&cProviderScreenGradient, SCREEN_GRADIENT, "A Rectangle Gradient");
    publisher.publishProvider(&cProviderScreenModel,    SCREEN_MODEL,    "A 3D Model");
    publisher.publishProvider(&cProviderScreenPanel,    SCREEN_PANEL,    "A Rounded Rectangle");
    publisher.publishProvider(&cProviderScreenText,     SCREEN_TEXT,     "A Text Label or Value");
    publisher.publishProvider(&cProviderStringTime,     STRING_TIME,     "A Time Value");
  }
  
  void UI::updateInputs(unsigned int milliseconds) {
    // Nothing to do.
  }
  
  void UI::updateRuntime(unsigned int milliseconds) {
    updateRuntime2(cComponentTypeMenu,            milliseconds);
    updateRuntime2(cComponentTypeThrobber,        milliseconds);
    updateRuntime2(cComponentTypeVirtualKeyboard, milliseconds);
    for (std::unique_ptr<ScreenModel>& mScreenModel : cProviderScreenModel) {
      mScreenModel->updateRuntime(milliseconds);
    }
  }
  
  void UI::updateEditing(unsigned int milliseconds) {
    updateEditing2(cComponentTypeLayout, milliseconds);
    for (std::unique_ptr<ScreenModel>& mScreenModel : cProviderScreenModel) {
      mScreenModel->updateEditing(milliseconds);
    }
  }
  
  void UI::reset() {
    reset2(cComponentTypeLayout);
    reset2(cComponentTypeMenu);
    reset2(cComponentTypePrompt);
    reset2(cComponentTypeVirtualKeyboard);
  }  
  
  std::mutex cModuleInstantiationMutex;
  std::vector<std::unique_ptr<UI>> ModuleInstances;
}
 
#ifdef __linux__
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::Project* project, IsoRealms::IComponentTypeRegistry* registry) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::Project * project, IsoRealms::IComponentTypeRegistry * registry) {
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
