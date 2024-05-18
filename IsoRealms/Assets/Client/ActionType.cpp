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
#include "ActionType.h"

namespace IsoRealms {
  ActionType::ActionType(IProject* project, std::function<void()> relinquishInstances) :
            cProject(project),
            cActionType(cProject->createLiteralActionType(this)),
            cRelinquishInstances(relinquishInstances) {
  }

  ActionType::ActionType(IProject* project, std::function<void()> relinquishInstances, JSONObject object) :
            cProject(project),
            cActionType(cProject->getActionType(this, object)),
            cRelinquishInstances(relinquishInstances) {
  }

  void ActionType::init(JSONObject object, const std::string& member) {
    cProject->init([this, object, member](IAssets* assets) {
      set(object, member);
    });
  }

  void ActionType::set(JSONObject object, const std::string& member) {
    JSONObject mAssetObject = object.getObject(member);
    cProject->release(this, cActionType);
    cActionType= cProject->getActionType(this, mAssetObject);
  }

  void ActionType::save(JSONObject object, const std::string& name) const {
    JSONObject mAssetObject = object.addObject(name);
    cProject->save(mAssetObject, cActionType);
  }

  void ActionType::relinquish(IActionType* asset) {
    if (cActionType == asset) {
      cActionType = cProject->createLiteralActionType(this);
      cRelinquishInstances();
    }
  }

  ActionType::~ActionType() {
    if (cActionType != nullptr) {
      cProject->release(this, cActionType);
    }
  }
}
