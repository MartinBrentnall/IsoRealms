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
#include "Vertex.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Project.h"

namespace IsoRealms {
  Vertex::Vertex(IResourceData& owner) :
            Asset<Vertex, IVertex, IResourceData>(owner, owner.getAssetManager().createLiteralVertex(this, owner, 0.0f, 0.0f, 0.0f)) {
  }

  IVertex* Vertex::createLiteralAsset(IResourceData& owner) {
    return owner.getAssetManager().createLiteralVertex(this, owner, 0.0f, 0.0f, 0.0f);
  }
  
  IVertex* Vertex::getAsset(IResourceData& owner, JSONObject object) {
    return owner.getAssetManager().getVertex(this, object, owner);
  }
  
  IVertex* Vertex::getAsset(IResourceData& owner, const std::string& id) {
    return owner.getAssetManager().getVertex(this, id, owner);
  }
  
  bool Vertex::isDefaultConfiguration() const {
    return true;
  }
}
