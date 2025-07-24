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
#include "Integer.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Editing/Property/IPropertyManager.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Project.h"

namespace IsoRealms {
  Integer::Integer(IResourceData& owner) :
            Asset<Integer, IInteger, IResourceData>(owner, owner.getAssetManager().createLiteralInteger(this, owner, 0)) {
  }

  IInteger* Integer::createLiteralAsset(IResourceData& owner) {
    return owner.getAssetManager().createLiteralInteger(this, owner, 0);
  }
  
  IInteger* Integer::getAsset(IResourceData& owner, JSONObject object) {
    return owner.getAssetManager().getInteger(this, object, owner, nullptr);
  }
  
  IInteger* Integer::getAsset(IResourceData& owner, const std::string& id) {
    return owner.getAssetManager().getInteger(this, id, owner, nullptr);
  }
  
  bool Integer::isDefaultConfiguration() const {
    return true;
  }
}
