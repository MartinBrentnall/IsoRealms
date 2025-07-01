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

#include "IsoRealms/Assets/Type/IAction.h"
#include "IsoRealms/Assets/Type/IActionType.h"
#include "IsoRealms/Persistence/JSONDocument.h"

namespace IsoRealms {
  class IResourceData;

  class DummyActionType : public IActionType {
    public:
    DummyActionType(IResourceData& owner);
      
    /**************************\
     * Implements IActionType *
    \**************************/
    IAction* createAction(JSONObject object, IResourceData& owner, IBindingRegistry* localArgs) override;
    IAction* createAction(IResourceData& owner, IBindingRegistry* localArgs) override;
    void destroyAction(IAction* action, IAssets& assets) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:
    class Action : public IAction {
      public:
      Action();
      
      /**********************\
       * Implements IAction *
      \**********************/
      void execute() override;
      void save(JSONObject object) const override;
      bool hasConfiguration() const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;
    };
    Action DUMMY;
  };
}
