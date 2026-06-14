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
  ProjectConfigurer::ProjectConfigurer(Basics& basics, IComponentData& data) :
            cActionContext(data, *this),
            cDefFont(data),
            cDefCodeFont(data),
            cDefExitAction(cActionContext),
            cDefEditorAction(cActionContext),
            cProjectConfigurationUI(data.getProject(), *this, [this]() {
              cDefExitAction.execute();
            }, [this](IEditable* editor) {
              cBindingEditor.setValue(editor);
              cDefEditorAction.execute();
              cBindingEditor.setValue(nullptr);
            }),
            cLuaBinding(data.getProject().getLuaState(), this),
            cBindingEditor(data.getProject().getLuaState(), nullptr, nullptr, true) {
  }

  void ProjectConfigurer::define(IComponentDefiner& definer) {
    definer.propertyResource("font",         cDefFont);
    definer.propertyFloat(   "fontSize",     [this]() {return cDefFontSize;},     [this](float value) {cDefFontSize     = value;});
    definer.propertyResource("codeFont",     cDefCodeFont);
    definer.propertyFloat(   "codeFontSize", [this]() {return cDefCodeFontSize;}, [this](float value) {cDefCodeFontSize = value;});
    definer.propertyResource("onExit",       cDefExitAction);
    definer.propertyResource("onEditor",     cDefEditorAction);
    // TODO: Input configuration
  }

  void ProjectConfigurer::publish(ResourcePublisher& publisher) {
    publisher.publish<IScreen>(this, "", "Project Configurers");
    publisher.publish<IInputHandler>(this, "", "Project Configurers");
    publisher.publish<IBinding>(&cLuaBinding, "", "Project Configurers");
  }
  void ProjectConfigurer::updateRuntime(unsigned int milliseconds) {
    cProjectConfigurationUI.update(milliseconds);
  }
    
  void ProjectConfigurer::renderScreen(float scale, float aspectRatio) const {
    cProjectConfigurationUI.render(aspectRatio);
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

  std::string ProjectConfigurer::getBindingID(const IBinding* binding) const {
    if (binding == &cBindingEditor) {
      return "Editor";
    }
    return "";
  }

  IBinding* ProjectConfigurer::getBinding(const std::string& id) {
    return id == "Editor" ? &cBindingEditor : nullptr;
  }

  void ProjectConfigurer::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    getTreeItemInfoFunction(TreeItemInfo{"editor", "editor"});
  }

  void ProjectConfigurer::releaseBinding(const IBinding* resource) {
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
