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
#include "BoundAction.h"

#include "IsoRealms/Lua/LuaState.h"

namespace IsoRealms {
  const std::string BoundAction::TAG_ACTION = "Action";
  
  BoundAction::BoundAction(IProject* project, DOMNode& node) :
            cDefLuaState(project->getLuaState()->getState()),
            cDefAction(project) {
    cDefAction.init(node, TAG_ACTION, nullptr, "");
  }

  void BoundAction::bind(const std::string& bindFunction) const {
    (*cDefLuaState)[bindFunction](&cDefAction);
  }

  void BoundAction::save(DOMNodeWriter* node, IBindingRegistry* localBindings, IAssetIdentifier* identifier, const std::string& attribute) const {
    node->addAttribute(attribute, ":Action");
    cDefAction.save(node, TAG_ACTION);
  }

  void BoundAction::release(IAssets* releaser) {
    // Nothing to do.
  }
}
