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
#pragma once

#include <algorithm>

#include "Modules/Spindizzy/Assets/Type/IWorldEditorTool.h"
#include "Modules/Spindizzy/IWorldObject.h"

namespace IsoRealms::Spindizzy {
  class DeleteTool : public IWorldEditorTool {
    public:

    /*******************************\
     * Implements IWorldEditorTool *
    \*******************************/
    IWorldEditorToolInstance* createToolInstance(WorldEditor* editor) override;
    bool renderAssetIcon() const override;

    private:

    // Internal classes.
    class Eraser : public IWorldEditorToolInstance {
      public:
      Eraser(DeleteTool& parent, WorldEditor* editor);

      /***************************************\
       * Implements IWorldEditorToolInstance *
      \***************************************/
      bool isTool(IWorldEditorTool* tool) const override;
      bool renderIcon(float yaw) const override;
      void renderEditingPreview() const override;
      void renderUI(float aspectRatio) const override;
      void updateUI(unsigned int milliseconds) override;
      bool inputEdit(sf::Event& event, double yaw) override;
      void processCursorMovement(LiteralVertex* start, LiteralVertex* end) override;
      double getSnapInterval() const override;

      private:
      DeleteTool& cParent;
      WorldEditor* cEditor;
      std::vector<IWorldObject*> cHoverObjects;
      int cSelectedObject;
      void removeSelectedObject();
    };

    // Editing data.
    std::vector<std::unique_ptr<Eraser>> cEditingErasers; /// Active erasers.
  };
}
