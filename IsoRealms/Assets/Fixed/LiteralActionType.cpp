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
#include "LiteralActionType.h"

namespace IsoRealms {
  LiteralActionType::LiteralActionType() :
    DUMMY(this) {
  }
  
  IAction* LiteralActionType::createAction(JSONObject object, IProject* project, IBindingRegistry* localObjects) {
    return &DUMMY;
  }

  IAction* LiteralActionType::createAction(IProject* project, IBindingRegistry* localArgs) {
    return &DUMMY;
  }
  
  void LiteralActionType::destroyAction(IAction* action, IAssets* assets) {
    // Nothing to do.
  }

  bool LiteralActionType::renderAssetIcon() const {
    return false;
  }
  
  void LiteralActionType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  LiteralActionType::Action::Action(LiteralActionType* parent) :
            cParent(parent) {
  }
  
  void LiteralActionType::Action::execute() {
    // Do nothing.
  }
  
  IActionType* LiteralActionType::Action::getActionType() const {
    return cParent;
  }
  
  void LiteralActionType::Action::save(JSONObject object, IAssetIdentifier* identifier) const {
    // Nothing to do.
  }

  bool LiteralActionType::Action::hasConfiguration() const {
    return false;
  }  
}
