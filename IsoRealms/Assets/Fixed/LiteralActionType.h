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

namespace IsoRealms {
  class LiteralActionType : public IActionType {
    public:
    LiteralActionType();
      
    /**************************\
     * Implements IActionType *
    \**************************/
    IAction* createAction(DOMNode& node, IProject* project, IBindingRegistry* localObjects) override;
    IAction* createAction(IProject* project, IBindingRegistry* localArgs) override;
    void destroyAction(IAction* action, IAssets* assets) override;
    bool renderAssetIcon() const override;

    private:
    class Action : public IAction {
      private:
      LiteralActionType* cParent;
      
      public:
      Action(LiteralActionType* parent);
      
      /**********************\
       * Implements IAction *
      \**********************/
      void execute() override;
      IActionType* getActionType() const override;
      void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const override;
      bool hasConfiguration() const override;
    };
    Action DUMMY;
  };
}
