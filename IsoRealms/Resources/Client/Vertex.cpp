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
#include "Vertex.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/Project/Project.h"

namespace IsoRealms {
  Vertex::Vertex(IComponentData& owner) :
            Resource<Vertex, IVertex, IComponentData>(owner, owner.getResourceManager().createLiteralVertex(this, owner, 0.0f, 0.0f, 0.0f)) {
  }

  IVertex* Vertex::createDefaultResource(IComponentData& owner) {
    return owner.getResourceManager().createLiteralVertex(this, owner, 0.0f, 0.0f, 0.0f);
  }
}
