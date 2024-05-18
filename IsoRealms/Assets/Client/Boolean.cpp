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
#include "Boolean.h"

namespace IsoRealms {
  Boolean::Boolean(IProject* project, bool defaultValue, std::function<void(bool)> listener) :
            cProject(project),
            cDefaultValue(defaultValue),
            cBoolean(cProject->createLiteralBoolean(this, cDefaultValue)),
            cListener(listener) {
  }

  void Boolean::init(JSONObject object, const std::string& member) {
    cProject->init([this, object, member](IAssets* assets) {
      set(object, member);
    });
  }

  void Boolean::set(JSONObject object, const std::string& member) {
    JSONObject mAssetObject = object.getObject(member);
    cProject->release(this, cBoolean);
    cBoolean = cProject->getBoolean(this, mAssetObject, cListener != nullptr ? this : nullptr);
  }

  void Boolean::save(JSONObject object, const std::string& name) const {
    JSONObject mAssetObject = object.addObject(name);
    cProject->save(mAssetObject, cBoolean);
  }

  void Boolean::relinquish(IBoolean* asset) {
    if (cBoolean == asset) {
      cBoolean = cProject->createLiteralBoolean(this, cDefaultValue);
    }
  }

  void Boolean::stateChanged(IBoolean* boolean) {
    if (boolean == cBoolean) {
      cListener(cBoolean->getValue());
    }
  }

  Boolean::~Boolean() {
    if (cBoolean != nullptr) {
      cProject->release(this, cBoolean);
    }
  }
}

