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

#include "Modules/Equilibria/Assets/Type/IWorldEditorTool.h"
#include "Modules/Equilibria/IWorldObject.h"

namespace IsoRealms::Equilibria {
  class ZoneTool : public IWorldEditorTool {
    public:
    enum class Type {
      COPY,
      MOVE,
      DELETE
    };

    ZoneTool(Type type);

    /*******************************\
     * Implements IWorldEditorTool *
    \*******************************/
    IWorldEditorToolInstance* createToolInstance(WorldEditor& editor, IResourceData& owner) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:

    // Internal classes.
    class Instance : public IWorldEditorToolInstance {
      public:
      Instance(ZoneTool& parent, WorldEditor& editor);

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

      private:
      ZoneTool& cParent;
      WorldEditor& cEditor;
      Zone* cHoverZone;
      Zone* cSelectedZone;
    };

    // Definition data.
    Type cDefType;

    // Editing data.
    std::vector<std::unique_ptr<Instance>> cInstances; /// Active instances.

    static bool renderEditingIcon(Type type);
  };
}
