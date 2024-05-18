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
#include "Font.h"

namespace IsoRealms {
  Font::Font(IProject* project) :
            cProject(project),
            cFont(cProject->createLiteralFont(this)) {
  }

  void Font::init(JSONObject object, const std::string& member) {
    cProject->init([this, object, member](IAssets* assets) {
      set(object, member);
    });
  }

  void Font::set(JSONObject object, const std::string& member) {
    JSONObject mAssetObject = object.getObject(member);
    cProject->release(this, cFont);
    cFont = cProject->getFont(this, mAssetObject);
  }

  void Font::save(JSONObject object, const std::string& name) const {
    JSONObject mAssetObject = object.addObject(name);
    cProject->save(mAssetObject, cFont);
  }

  void Font::relinquish(IFont* asset) {
    if (cFont == asset) {
      cFont = cProject->createLiteralFont(this);
    }
  }

  Font::~Font() {
    if (cFont != nullptr) {
      cProject->release(this, cFont);
    }
  }
}
