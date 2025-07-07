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
#include "Action.h"

#include "IsoRealms/Project.h"
#include "IsoRealms/ActionClient.h"
#include "IsoRealms/Editing/Property/IProperty.h"

namespace IsoRealms {
  Action::Action(IActionClient& owner) :
            Asset<Action, IAction, IActionClient>(owner, owner.getAssetManager().createDefault(this, owner)) {
  }

  void Action::execute() {
    if (cManager.getProject().isProcessingInput()) {
      cManager.getProject().postponeAction(cAsset);
    } else {
      cAsset->execute();
    }
  }

  IAction* Action::createLiteralAsset(IActionClient& owner) {
    return owner.getAssetManager().createDefault(this, owner);
  }
  
  IAction* Action::getAsset(IActionClient& owner, JSONObject object) {
    return owner.getAssetManager().getAction(this, object, owner);
  }
  
  IAction* Action::getAsset(IActionClient& owner, const std::string& id) {
    return owner.getAssetManager().getAction(this, id, owner);
  }
  
  bool Action::isDefaultConfiguration() const {
    return true;
  }
}
