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
#include "Assets.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Project.h"

namespace IsoRealms {
  Assets::Assets(IResourceData& owner) :
            Asset<Assets, IAssets, IResourceData>(owner, owner.getAssetManager().createDefault(this, owner)) {
  }

  IAssets* Assets::createLiteralAsset(IResourceData& owner) {
    return owner.getAssetManager().createDefault(this, owner);
  }
  
  IAssets* Assets::getAsset(IResourceData& owner, JSONObject object) {
    return owner.getAssetManager().getAssets(this, object, owner);
  }
  
  IAssets* Assets::getAsset(IResourceData& owner, const std::string& id) {
    return owner.getAssetManager().getAssets(this, id, owner);
  }
  
  bool Assets::isDefaultConfiguration() const {
    return true;
  }
}
