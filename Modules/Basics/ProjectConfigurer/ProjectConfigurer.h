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
                                  public IBindingRegistry {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    ProjectConfigurer(IProject& project, Basics& basics, IResourceData& data);
    ProjectConfigurer(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(PropertyMaker& owner, const Metadata& metadata);

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
    void getAssetProperties(PropertyMaker& owner) override;
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
    IProject& getProject() const override;

    /*******************************\
     * Implements IBindingRegistry *
    \*******************************/
    IBinding* getBinding(const std::string& id) override;
    void saveBinding(JSONObject object, const IBinding* binding) const override;
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

    // TODO: Duplicated from WorldEditor
    class DigitalInput {
      public:
      DigitalInput(IResourceData& owner, ProjectConfigurer& parent, SignalInputID signal) :
                cParent(parent),
                cInput(owner, false, [this](bool value) {
                  if (value && !cValue) {
                    cParent.signal(cSignal);
                  }
                  cValue = value;
                }),
                cValue(false),
                cSignal(signal) {
      }

      void set(JSONObject object) {
        cInput.init(object, JSON_MAPPING);
      }

      bool get() {
        return cValue;
      }

      private:
      ProjectConfigurer& cParent;
      Boolean cInput;
      bool cValue;
      SignalInputID cSignal;
    };

    // Private functions.
    bool signal(SignalInputID id);

    // JSON members.
    static const std::string JSON_CODE_FONT;
    static const std::string JSON_CODE_FONT_SIZE;
    static const std::string JSON_FONT;
    static const std::string JSON_FONT_SIZE;
    static const std::string JSON_INPUT;
    static const std::string JSON_LOCAL;
    static const std::string JSON_MAPPING;
    static const std::string JSON_ON_EDITOR;
    static const std::string JSON_ON_EXIT;

    // Action client.
    ActionClient cActionClient;

    // Definition data.
    Font cDefFont;
    Font cDefCodeFont;
    float cDefFontSize;
    float cDefCodeFontSize;
    Action cDefExitAction;
    Action cDefEditorAction;

    UIManager cProjectConfigurationUI;

    const std::map<std::string, DigitalInput*> cDigitalInputsByName; /// Mapping of digital inputs by name.

    DigitalInput cAdjustLeft;
    DigitalInput cAdjustRight;
    DigitalInput cCancel;
    DigitalInput cConfirm;
    DigitalInput cPreviousItem;
    DigitalInput cNextItem;

    // Scripting Interface.
    LuaBinding<ProjectConfigurer> cLuaBinding;
    
    LocalLuaBinding<IEditable> cBindingEditor;
  };
}
