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
#include "String.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Project.h"

namespace IsoRealms {
  String::String(IResourceData& owner) :
            Asset<String, IString, IResourceData>(owner, owner.getAssetManager().createLiteralString(this, owner, "")) {
  }

  IString* String::createLiteralAsset(IResourceData& owner) {
    return owner.getAssetManager().createLiteralString(this, owner, "");
  }
  
  IString* String::getAsset(IResourceData& owner, JSONObject object) {
    return owner.getAssetManager().getString(this, object, owner, nullptr);
  }
  
  IString* String::getAsset(IResourceData& owner, const std::string& id) {
    return owner.getAssetManager().getString(this, id, owner, nullptr);
  }
  
  bool String::isDefaultConfiguration() const {
    return true;
  }
}
