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
#include "Action.h"

namespace IsoRealms {
  Action::Action(IProject* project) :
            cProject(project),
            cAction(cProject->createLiteralAction(this)) {
  }

  void Action::init(JSONObject object, const std::string& tag, IBindingRegistry* localArgs, const std::string& id) {
    if (object.hasMember(tag)) {
      cProject->init([this, object, tag, localArgs, id](IAssets* assets) {
        set(object, tag, localArgs, id);
      });
    }
  }

  void Action::set(JSONObject object, const std::string& tag, IBindingRegistry* localArgs, const std::string& id) {
    cProject->release(this, cAction);
    cAction = cProject->getAction(this, object, tag, localArgs, id);
  }

  void Action::execute() {
    cAction->execute();
  }

  void Action::save(JSONObject object, const std::string& name) const {
    JSONObject mAssetObject = object.addObject(name);
    cAction->save(mAssetObject, cProject);
  }

  void Action::relinquish(IAction* asset) {
    if (cAction == asset) {
      cAction = cProject->createLiteralAction(this);
    }
  }

  Action::~Action() {
    if (cAction != nullptr) {
      cProject->release(this, cAction);
    }
  }
}
