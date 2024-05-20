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
#include "MenuItem.h"

#include "Modules/UI/UI.h"

namespace IsoRealms::UI {
  MenuItem::MenuItem(UI* ui) :
            cUI(ui),
            cMenuItem(cUI->createLiteralMenuItem(this)) {
  }

  void MenuItem::init(JSONObject object, const std::string& member, Menu* owner) {
    cUI->getProject()->init([this, member, object, owner](IAssets* assets) {
      std::cout << "==============================================================+!!!!!!!!!" << std::endl;
      set(object, member, owner);
    });
  }

  void MenuItem::set(JSONObject object, const std::string& member, Menu* owner) {
    JSONObject mAssetObject = object.getObject(member);
    cUI->release(this, cMenuItem);
    cMenuItem = cUI->getMenuItem(this, mAssetObject, owner);
  }

  void MenuItem::save(JSONObject object, const std::string& name) const {
    JSONObject mAssetObject = object.addObject(name);
    cUI->save(mAssetObject, cMenuItem);
  }

  void MenuItem::relinquish(IMenuItem* asset) {
    if (cMenuItem == asset) {
      cMenuItem = cUI->createLiteralMenuItem(this);
    }
  }

  MenuItem::~MenuItem() {
    if (cMenuItem != nullptr) {
      cUI->release(this, cMenuItem);
    }
  }
}
