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
#include "ProjectConfigurer.h"

#include "Modules/Basics/Basics.h"

namespace IsoRealms::Basics {
  const std::string ProjectConfigurer::JSON_CODE_FONT      = "codeFont";
  const std::string ProjectConfigurer::JSON_CODE_FONT_SIZE = "codeFontSize";
  const std::string ProjectConfigurer::JSON_FONT           = "font";
  const std::string ProjectConfigurer::JSON_FONT_SIZE      = "fontSize";
  const std::string ProjectConfigurer::JSON_LOCAL          = "local";
  const std::string ProjectConfigurer::JSON_ON_EDITOR      = "onEditor";
  const std::string ProjectConfigurer::JSON_ON_EXIT        = "onExit";

  ProjectConfigurer::ProjectConfigurer(Basics& basics, IResourceData& data) :
            cActionClient(data, *this),
            cDefFont(data),
            cDefCodeFont(data),
            cDefFontSize(0.03f),
            cDefCodeFontSize(0.02f),
            cDefExitAction(cActionClient),
            cDefEditorAction(cActionClient),
            cProjectConfigurationUI(data.getProject(), *this, [this]() {
              cDefExitAction.execute();
            }, [this](IEditable* editor) {
              cBindingEditor.setValue(editor);
              cDefEditorAction.execute();
              cBindingEditor.setValue(nullptr);
            }),
            cLuaBinding(data.getProject().getLuaState(), this),
            cBindingEditor(data.getProject().getLuaState(), nullptr, this) {
  }

  ProjectConfigurer::ProjectConfigurer(Basics& basics, IResourceData& data, JSONObject object) :
            ProjectConfigurer(basics, data) {
    cDefFontSize = object.getFloat(JSON_FONT_SIZE);
    cDefCodeFontSize = object.getFloat(JSON_CODE_FONT_SIZE);
    cDefFont.init(object, JSON_FONT);
    cDefCodeFont.init(object, JSON_CODE_FONT);
    cDefExitAction.init(object, JSON_ON_EXIT);
    cDefEditorAction.init(object, JSON_ON_EDITOR);
  }

  void ProjectConfigurer::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IScreen>(this, "", "Project Configurers");
    assets.add<IInputHandler>(this, "", "Project Configurers");
    assets.add<IBinding>(&cLuaBinding, "", "Project Configurers");
  }

  void ProjectConfigurer::save(JSONObject object) const {
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

  void ProjectConfigurer::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyAsset<Font>(  metadata.getPropertyData("Font"),         cDefFont);
    owner.createPropertyNativeFloat(  metadata.getPropertyData("FontSize"),     [this]() {return cDefFontSize;},     [this](float value) {cDefFontSize     = value;});
    owner.createPropertyAsset<Font>(  metadata.getPropertyData("CodeFont"),     cDefCodeFont);
    owner.createPropertyNativeFloat(  metadata.getPropertyData("CodeFontSize"), [this]() {return cDefCodeFontSize;}, [this](float value) {cDefCodeFontSize = value;});
    owner.createPropertyAsset<Action>(metadata.getPropertyData("OnExit"),       cDefExitAction);
    owner.createPropertyAsset<Action>(metadata.getPropertyData("OnEditor"),     cDefEditorAction);
    // TODO: Input configuration
  }

  void ProjectConfigurer::updateRuntime(unsigned int milliseconds) {
    cProjectConfigurationUI.update(milliseconds);
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

  void ProjectConfigurer::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool ProjectConfigurer::isDefaultConfiguration() const {
    return true;
  }

  bool ProjectConfigurer::input(sf::Event& event) {
    if (!isHidden()) {
      if (cProjectConfigurationUI.input(event)) {
        return true;
      }
    }
    return false;
  }
  
  IDialogManager& ProjectConfigurer::getDialogManager() {
    return cProjectConfigurationUI;
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
  
  IsoRealms::Project& ProjectConfigurer::getProject() const {
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
    cProjectConfigurationUI.openUI(std::make_unique<ProjectMenu>(cProjectConfigurationUI, *this, project), "");
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
}
