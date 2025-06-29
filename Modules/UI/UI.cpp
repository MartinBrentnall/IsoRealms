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
  const std::string UI::ID_RESOURCE_PROMPT           = "Prompt";
  const std::string UI::ID_RESOURCE_THROBBER         = "Throbber";
  const std::string UI::ID_RESOURCE_VIRTUAL_KEYBOARD = "VirtualKeyboard";

  const std::string UI::MENU_ITEM_ACTION             = "Action";
  const std::string UI::MENU_ITEM_BOOLEAN            = "Boolean";
  const std::string UI::MENU_ITEM_DIGITAL_INPUT      = "DigitalInput";
  const std::string UI::MENU_ITEM_DISPLAY_RESOLUTION = "DisplayResolution";
  const std::string UI::MENU_ITEM_FILE_LIST          = "FileList";
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

  UI::UI(IProject& project, IResourceTypeRegistry* registry):
                    cProject(project),
                    cProviderLayoutLocationAbsolute(project),
                    cProviderLayoutLocationRelative(project),
                    cProviderLayoutOffsetAbsolute(project),
                    cProviderLayoutOffsetLinked(project),
                    cProviderMenuItemAction(project),
                    cProviderMenuItemBoolean(project),
                    cProviderMenuItemDigitalInput(project),
                    cProviderMenuItemDisplayResolution(project),
                    cProviderMenuItemFileList(project),
                    cProviderMenuItemSlider(project),
                    cProviderScreenGradient(project),
                    cProviderScreenModel(project),
                    cProviderScreenPanel(project),
                    cProviderScreenText(project),
                    cProviderStringTime(project),
                    cResourceTypeLayout(*this),
                    cResourceTypeMenu(*this),
                    cResourceTypePrompt(*this),
                    cResourceTypeThrobber(*this),
                    cResourceTypeVirtualKeyboard(*this) {
    registry->add(&cResourceTypeLayout,          ID_RESOURCE_LAYOUT,           "Layout",           "Layouts",           IsoRealmsConstants::RESOURCE_CATEGORY_PRESENTATION);
    registry->add(&cResourceTypeMenu,            ID_RESOURCE_MENU,             "Menu",             "Menus",             IsoRealmsConstants::RESOURCE_CATEGORY_PRESENTATION);
    registry->add(&cResourceTypePrompt,          ID_RESOURCE_PROMPT,           "Prompt",           "Prompts",           IsoRealmsConstants::RESOURCE_CATEGORY_PRESENTATION);
    registry->add(&cResourceTypeThrobber,        ID_RESOURCE_THROBBER,         "Throbber",         "Throbbers",         IsoRealmsConstants::RESOURCE_CATEGORY_PRESENTATION);
    registry->add(&cResourceTypeVirtualKeyboard, ID_RESOURCE_VIRTUAL_KEYBOARD, "Virtual Keyboard", "Virtual Keyboards", IsoRealmsConstants::RESOURCE_CATEGORY_PRESENTATION);

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
    cMenuItems.add(&cProviderMenuItemSlider,            MENU_ITEM_SLIDER,             "UI");
  }

  IUI& UI::getAssetManager() {
    return *this;
  }

  IProject& UI::getProject() const {
    return cProject;
  }

  bool UI::isReadOnly() const {
    return false;
  } // TODO: Probably shouldn't be here.

  void UI::setOwner(File* owner) {
  } // TODO: Probably shouldn't be here.

  std::vector<std::string> UI::getAllLayoutLocations() {return cLayoutLocations.getAll();}
  std::vector<std::string> UI::getAllLayoutOffsets()   {return cLayoutOffsets.getAll();}
  std::vector<std::string> UI::getAllMenuItems()       {return cMenuItems.getAll();}

  std::string UI::getID(const ILayoutLocation* asset) const {return cLayoutLocations.getID(asset);}
  std::string UI::getID(const ILayoutOffset*   asset) const {return cLayoutOffsets.getID  (asset);}
  std::string UI::getID(const IMenuItem*       asset) const {return cMenuItems.getID(      asset);}

  bool UI::renderLayoutLocationIcon(const std::string& id) const {return cLayoutLocations.renderIcon(id);}
  bool UI::renderLayoutOffsetIcon(  const std::string& id) const {return cLayoutOffsets.renderIcon(  id);}
  bool UI::renderMenuItemIcon(      const std::string& id) const {return cMenuItems.renderIcon(      id);}

  bool UI::isLayoutLocationConfigurable(const std::string& id) const {return cLayoutLocations.hasConfiguration(id);}
  bool UI::isLayoutOffsetConfigurable(  const std::string& id) const {return cLayoutOffsets.hasConfiguration(  id);}
  bool UI::isMenuItemConfigurable(      const std::string& id) const {return cMenuItems.hasConfiguration(      id);}

  void UI::load(IProject& project, JSONObject object) {
    // Nothing to do.
  }

  void UI::save(JSONObject object, IAssetIdentifier& identifier) {
    // Nothing to do.
  }

  void UI::registerAssets(IAssetRegistry& assets) {
    assets.add(&cProviderScreenGradient, SCREEN_GRADIENT, "");
    assets.add(&cProviderScreenModel,    SCREEN_MODEL,    "");
    assets.add(&cProviderScreenPanel,    SCREEN_PANEL,    "");
    assets.add(&cProviderScreenText,     SCREEN_TEXT,     "");
    assets.add(&cProviderStringTime,     STRING_TIME,     "");
  }
  
  std::vector<std::unique_ptr<IProperty>> UI::getProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }  

  ILayoutLocation* UI::createLiteralLayoutLocation(IAssetUser<ILayoutLocation>* user, LayoutComponentEdge& owner) {return cLayoutLocations.literal(user, owner, "");}
  ILayoutOffset*   UI::createLiteralLayoutOffset(  IAssetUser<ILayoutOffset>*   user, LayoutComponentEdge& owner) {return cLayoutOffsets.literal(  user, owner, "");}
  IMenuItem*       UI::createLiteralMenuItem(      IAssetUser<IMenuItem>*       user, Menu&                owner) {return cMenuItems.literal(      user, owner, "");}

  ILayoutLocation* UI::getLayoutLocation(IAssetUser<ILayoutLocation>* user, JSONObject object, LayoutComponentEdge& owner) {return cLayoutLocations.get(user, owner, object, nullptr, true, [this](JSONObject object, IStateListener<ILayoutLocation*>* listener) -> ILayoutLocation* {return nullptr;});}
  ILayoutOffset*   UI::getLayoutOffset(  IAssetUser<ILayoutOffset>*   user, JSONObject object, LayoutComponentEdge& owner) {return cLayoutOffsets.get(  user, owner, object, nullptr, true, [this](JSONObject object, IStateListener<ILayoutOffset*>*   listener) -> ILayoutOffset*   {return nullptr;});}
  IMenuItem*       UI::getMenuItem(      IAssetUser<IMenuItem>*       user, JSONObject object, Menu&                owner) {return cMenuItems.get(      user, owner, object, nullptr, true, [this](JSONObject object, IStateListener<IMenuItem*>*       listener) -> IMenuItem*       {return nullptr;});}

  ILayoutLocation* UI::getLayoutLocation(IAssetUser<ILayoutLocation>* user, const std::string& id, LayoutComponentEdge& owner) {return cLayoutLocations.get(user, owner, id, nullptr);}
  ILayoutOffset*   UI::getLayoutOffset(  IAssetUser<ILayoutOffset>*   user, const std::string& id, LayoutComponentEdge& owner) {return cLayoutOffsets.get(  user, owner, id, nullptr);}
  IMenuItem*       UI::getMenuItem(      IAssetUser<IMenuItem>*       user, const std::string& id, Menu&                owner) {return cMenuItems.get(      user, owner, id, nullptr);}

  void UI::release(IAssetUser<ILayoutLocation>* user, ILayoutLocation* asset) {cLayoutLocations.release(user, asset);}
  void UI::release(IAssetUser<ILayoutOffset>*   user, ILayoutOffset*   asset) {cLayoutOffsets.release(  user, asset);}
  void UI::release(IAssetUser<IMenuItem>*       user, IMenuItem*       asset) {cMenuItems.release(      user, asset);}

  void UI::save(JSONObject object, ILayoutLocation* asset) const {cLayoutLocations.save(object, asset);}
  void UI::save(JSONObject object, ILayoutOffset*   asset) const {cLayoutOffsets.save(  object, asset);}
  void UI::save(JSONObject object, IMenuItem*       asset) const {cMenuItems.save(      object, asset);}

  std::mutex cModuleInstantiationMutex;
  std::vector<std::unique_ptr<UI>> ModuleInstances;
}
 
#ifdef __linux__
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::IProject* project, IsoRealms::IResourceTypeRegistry* registry) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::IProject * project, IsoRealms::IResourceTypeRegistry * registry) {
#endif
  std::unique_ptr<IsoRealms::UI::UI> mModule = std::make_unique<IsoRealms::UI::UI>(*project, registry);
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
