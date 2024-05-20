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

#include "IsoRealms/Assets/Type/IEditable.h"

namespace IsoRealms {
  class LiteralEditable : public IEditable {
    public:

    /************************\
     * Implements IEditable *
    \************************/
    IEditableScreen* createEditableScreen(Project* project) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;

    private:
    class EditableScreen : public IEditableScreen {
      public:

      /******************************\
       * Implements IEditableScreen *
      \******************************/
      void notifyVisible() override;
      void notifyHidden() override;
      void notifyLostFocus() override;
      void unregisterAssets(IAssetRemover* assets) override;
      void setAppearance(IFont* font, float scale) override;
      IScreen* screen() override;
      void renderScreen(float scale, float aspectRatio) const override;
      const IFloat* getYaw() const override;
      const IFloat* getPitch() const override;
      bool input(sf::Event& event) override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
    };
    static EditableScreen DUMMY; /// Dummy editable screen.
  };
}
