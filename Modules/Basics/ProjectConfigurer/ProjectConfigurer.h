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

#include "IsoRealms.h"

#include "SignalInputID.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource that enables configuration of an IsoRealms project.
   */
  class ProjectConfigurer final : public IScreen,
                                  public IInputHandler,
                                  public IUIStyle,
                                  public IEventBindings {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    ProjectConfigurer(Basics& basics, IResourceData& data);
    ProjectConfigurer(Basics& basics, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    /*********************\
     * Module interfaces *
    \*********************/
    void updateRuntime(unsigned int milliseconds);

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    /****************************\
     * Implements IInputHandler *
    \****************************/
    bool input(sf::Event& event) override;
    void resetInput() override;

    /***********************\
     * Implements IUIStyle *
    \***********************/
    IFont* getFont() const override;
    float getFontSize() const override;
    IFont* getCodeFont() const override;
    float getCodeFontSize() const override;
    IsoRealms::Project& getProject() const override;

    /*****************************\
     * Implements IEventBindings *
    \*****************************/
    std::string getBindingID(const IBinding* binding) const override;
    IBinding* getBinding(const std::string& id) override;
    void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;
    void releaseBinding(const IBinding* asset) override;
    
    /***********************\
     * Scripting Interface *
    \***********************/
    void setProject(IsoRealms::Project& project);
    void hide();
    void show();
    bool isHidden() const;
    IDialogManager& getDialogManager();

    private:

    // JSON members.
    static const std::string JSON_CODE_FONT;
    static const std::string JSON_CODE_FONT_SIZE;
    static const std::string JSON_FONT;
    static const std::string JSON_FONT_SIZE;
    static const std::string JSON_ON_EDITOR;
    static const std::string JSON_ON_EXIT;

    // Action client.
    ActionContext cActionContext;

    // Definition data.
    Font cDefFont;
    Font cDefCodeFont;
    float cDefFontSize;
    float cDefCodeFontSize;
    Action cDefExitAction;
    Action cDefEditorAction;

    UIManager cProjectConfigurationUI;

    // Scripting Interface.
    LuaBinding<ProjectConfigurer> cLuaBinding;
    
    LuaBinding<IEditable> cBindingEditor;
  };
}
