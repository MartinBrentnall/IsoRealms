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

#include <functional>

#include "IsoRealms/Assets/Providers/AssetLiteralDummy.h"
#include "IsoRealms/Assets/Type/IEditable.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Utils.h"

#include "AssetClientManager.h"
#include "IAssetUser.h"

namespace IsoRealms {
  class Project;

  class EditableRegistry : public AssetClientManager<EditableRegistry, IResourceData, IEditable> {
    public:
    EditableRegistry();

    private:
    class Dummy : public IEditable {
      public:

      /************************\
      * Implements IEditable *
      \************************/
      IEditableScreen* createEditableScreen(Project* project, IDialogManager& dialogManager) override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(IPropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;

      private:
      class Instance : public IEditableScreen {
        public:

        /******************************\
        * Implements IEditableScreen *
        \******************************/
        void notifyVisible() override;
        void notifyHidden() override;
        void notifyLostFocus() override;
        void notifyGainedFocus() override;
        std::vector<std::string> getDigitalInputs() const override;
        std::vector<std::string> getAnalogueInputs() const override;
        void setDigitalInput(const std::string& name, IBoolean* input) override;
        void setAnalogueInput(const std::string& name, IFloat* input) override;
        void setExitAction(IAction* action) override;
        void setAppearance(IFont* font, float scale) override;
        IScreen* screen() override;
        void renderScreen(float scale, float aspectRatio) const override;
        const IFloat* getYaw() const override;
        const IFloat* getPitch() const override;
        bool input(sf::Event& event) override;
        void resetInput() override;
        bool renderAssetIcon() const override;
        void saveAsset(JSONObject object) const override;
        void getAssetProperties(IPropertyMaker& owner) override;
        bool isDefaultConfiguration() const override;
      };
      static Instance DUMMY; /// Dummy editable screen.
    };

    AssetLiteralDummy<IResourceData, IEditable, Dummy> cNone;
  };
}
