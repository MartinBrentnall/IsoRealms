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
#include "ProjectConfigurer.h"

namespace IsoRealms::Basics {
  const std::string ProjectConfigurer::JSON_CODE_FONT        = "codeFont";
  const std::string ProjectConfigurer::JSON_CODE_FONT_SIZE   = "codeFontSize";
  const std::string ProjectConfigurer::JSON_FONT             = "font";
  const std::string ProjectConfigurer::JSON_FONT_SIZE        = "fontSize";
  const std::string ProjectConfigurer::JSON_INPUT            = "input";
  const std::string ProjectConfigurer::JSON_LOCAL            = "local";
  const std::string ProjectConfigurer::JSON_ON_EDITOR        = "onEditor";
  const std::string ProjectConfigurer::JSON_ON_EXIT          = "onExit";
  const std::string ProjectConfigurer::JSON_MAPPING          = "mapping";

  ProjectConfigurer::ProjectConfigurer(IProject& project, Basics& basics, IResourceData& data) :
            cProjectCallbackManager(project),
            cDefFont(project),
            cDefCodeFont(project),
            cDefFontSize(0.03f),
            cDefCodeFontSize(0.02f),
            cDefExitAction(project),
            cDefEditorAction(project),
            cProjectConfigurationUI(project, *this, [this]() {
              cDefExitAction.execute();
            }, [this](IEditable* editor) {
              cBindingEditor.setValue(editor);
              cDefEditorAction.execute();
              cBindingEditor.setValue(nullptr);
            }),
            cDigitalInputsByName({
              {"AdjustLeft",   &cAdjustLeft},
              {"AdjustRight",  &cAdjustRight},
              {"Cancel",       &cCancel},
              {"Confirm",      &cConfirm},
              {"PreviousItem", &cPreviousItem},
              {"NextItem",     &cNextItem}
            }),
            cAdjustLeft(project, *this, SignalInputID::MOVE_CURSOR_LEFT),
            cAdjustRight(project, *this, SignalInputID::MOVE_CURSOR_RIGHT),
            cCancel(project, *this, SignalInputID::CANCEL),
            cConfirm(project, *this, SignalInputID::CONFIRM),
            cPreviousItem(project, *this, SignalInputID::MOVE_CURSOR_UP),
            cNextItem(project, *this, SignalInputID::MOVE_CURSOR_DOWN),
            cLuaBinding(project, this),
            cBindingEditor(project, nullptr, this) {
    cProjectCallbackManager.updateRuntime([this](unsigned int milliseconds) {
      cProjectConfigurationUI.update(milliseconds);
    });
  }

  ProjectConfigurer::ProjectConfigurer(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            ProjectConfigurer(project, basics, data) {
    JSONArray mInputArray = object.getArray("inputs");
    for (JSONObject mInput : mInputArray) {
      std::string mInputID = mInput.getString(JSON_INPUT);
      cDigitalInputsByName.find(mInputID)->second->set(mInput);
    }

    cDefFontSize = object.getFloat(JSON_FONT_SIZE);
    cDefCodeFontSize = object.getFloat(JSON_CODE_FONT_SIZE);
    cDefFont.init(object, JSON_FONT);
    cDefCodeFont.init(object, JSON_CODE_FONT);
    cDefExitAction.init(object, JSON_ON_EXIT);
    cDefEditorAction.init(object, JSON_ON_EDITOR, this);
  }

  void ProjectConfigurer::registerAssets(IAssetRegistry& assets) {
    assets.add(static_cast<IScreen*>(this), "", "Project Configurers");
    assets.add(static_cast<IInputHandler*>(this), "", "Project Configurers");
    assets.add(&cLuaBinding, "", "Project Configurers");
  }

  void ProjectConfigurer::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    assets.remove(static_cast<IScreen*>(this),       relinquish);
    assets.remove(static_cast<IInputHandler*>(this), relinquish);
    assets.remove(&cLuaBinding,                      relinquish);
  }

  void ProjectConfigurer::save(JSONObject object, IAssetIdentifier& identifier) const {
    object.addFloat(JSON_FONT_SIZE, cDefFontSize);
    object.addFloat(JSON_CODE_FONT_SIZE, cDefCodeFontSize);
    cDefFont.save(object, JSON_FONT);
    cDefCodeFont.save(object, JSON_CODE_FONT);
    cDefExitAction.save(object, JSON_ON_EXIT);
    cDefEditorAction.save(object, JSON_ON_EDITOR);
    // TODO: Save input configuration
  }

  void ProjectConfigurer::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool ProjectConfigurer::renderIcon() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> ProjectConfigurer::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Font>>(  "Regular Font",      "TODO", cDefFont));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>(  "Regular Font Size", "TODO", [this]() {return cDefFontSize;},     [this](float value) {cDefFontSize     = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Font>>(  "Code Font",         "TODO", cDefCodeFont));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>(  "Code Font Size",    "TODO", [this]() {return cDefCodeFontSize;}, [this](float value) {cDefCodeFontSize = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>("Action on Exit",    "TODO", cDefExitAction));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>("Action on Editor",  "TODO", cDefEditorAction));
    // TODO: Input configuration
    return mProperties;
  }

  void ProjectConfigurer::renderScreen(float scale, float aspectRatio) const {
    cProjectConfigurationUI.render(aspectRatio);
  }

  bool ProjectConfigurer::renderAssetIcon() const {
    return renderIcon();
  }

  void ProjectConfigurer::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> ProjectConfigurer::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ProjectConfigurer::isDefaultConfiguration() const {
    return true;
  }

  bool ProjectConfigurer::input(sf::Event& event) {
    return isHidden() ? false : cProjectConfigurationUI.input(event);
  }
  
  void ProjectConfigurer::resetInput() {
    // Nothing to do.
  }
  
  IFont* ProjectConfigurer::getFont() const {
    return *cDefFont;
  }

  float ProjectConfigurer::getFontSize() const {
    return cDefFontSize;
  }

  IFont* ProjectConfigurer::getCodeFont() const {
    return *cDefCodeFont;
  }

  float ProjectConfigurer::getCodeFontSize() const {
    return cDefCodeFontSize;
  }
  
  IProject& ProjectConfigurer::getProject() const {
    return cProjectConfigurationUI.getProject();
  }  

  IBinding* ProjectConfigurer::getBinding(const std::string& id) {
    return id == "editor" ? &cBindingEditor : nullptr;
  }

  void ProjectConfigurer::saveBinding(JSONObject object, const IBinding* binding) const {
    if (binding == &cBindingEditor) {
      object.addString(JSON_LOCAL, "editor");
    }
  }

  void ProjectConfigurer::releaseBinding(const IBinding* asset) {
    // Nothing to do.
  }

  void ProjectConfigurer::setProject(IsoRealms::Project& project) {
    cProjectConfigurationUI.openUI(std::make_unique<ProjectMenu>(cProjectConfigurationUI, *this, project));
  }
  
  void ProjectConfigurer::hide() {
    cProjectConfigurationUI.hide();
  }
  
  void ProjectConfigurer::show() {
    cProjectConfigurationUI.show();
  }
  
  bool ProjectConfigurer::isHidden() const {
    return cProjectConfigurationUI.isHidden();
  }

  bool ProjectConfigurer::signal(SignalInputID id) {
    if (!isHidden()) {
      switch (id) {
        case SignalInputID::MOVE_CURSOR_DOWN:  cProjectConfigurationUI.input(UISignalID::MOVE_DOWN);  return true;
        case SignalInputID::MOVE_CURSOR_UP:    cProjectConfigurationUI.input(UISignalID::MOVE_UP);    return true;
        case SignalInputID::MOVE_CURSOR_LEFT:  cProjectConfigurationUI.input(UISignalID::MOVE_LEFT);  return true;
        case SignalInputID::MOVE_CURSOR_RIGHT: cProjectConfigurationUI.input(UISignalID::MOVE_RIGHT); return true;
        case SignalInputID::CANCEL:            cProjectConfigurationUI.input(UISignalID::CANCEL);     return true;
        case SignalInputID::CONFIRM:           cProjectConfigurationUI.input(UISignalID::CONFIRM);    return true;
      }
    }
    return false;
  }
}
