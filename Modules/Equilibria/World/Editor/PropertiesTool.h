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

#include <algorithm>

#include "IsoRealms.h"

#include "Modules/Equilibria/Assets/Type/IWorldEditorTool.h"
#include "Modules/Equilibria/IWorldObject.h"

namespace IsoRealms::Equilibria {
  class WorldEditor;

  class PropertiesTool : public IWorldEditorTool {
    public:

    /*******************************\
     * Implements IWorldEditorTool *
    \*******************************/
    IWorldEditorToolInstance* createToolInstance(WorldEditor& editor, IComponentData& owner) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:

    // Internal classes.
    class Modifier : public IWorldEditorToolInstance,
                     public IUIStyle {
      public:
      Modifier(PropertiesTool& parent, IComponentData& owner, WorldEditor& editor);

      /***************************************\
       * Implements IWorldEditorToolInstance *
      \***************************************/
      bool isTool(IWorldEditorTool* tool) const override;
      bool renderIcon(float yaw) const override;
      void renderEditingPreview() const override;
      void renderUI(float aspectRatio) const override;
      void updateUI(unsigned int milliseconds) override;
      bool inputTool(sf::Event& event) override;
      bool inputTool(SignalInputID id, double yaw) override;
      bool isCursorLocked() const override;
      void processCursorMovement(LiteralVertex* start, LiteralVertex* end) override;
      double getSnapInterval() const override;

      /***********************\
       * Implements IUIStyle *
      \***********************/
      IFont* getFont() const override;
      float getFontSize() const override;
      IFont* getCodeFont() const override;
      float getCodeFontSize() const override;
      Project& getProject() const override;
      
      private:
      PropertiesTool& cParent;
      IComponentData& cWorldComponentOwner;
      WorldEditor& cEditor;
      std::vector<IWorldObject*> cHoverObjects;
      int cSelectedObject     = 0;
      bool cEditingProperties = false;
      UIManager cPropertiesUI;

      void showProperties();
    };

    // Editing data.
    std::vector<std::unique_ptr<Modifier>> cEditingModifiers; /// Active modifiers.
  };
}
