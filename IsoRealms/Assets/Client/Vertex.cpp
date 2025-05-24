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

namespace IsoRealms {
  Vertex::Vertex(IProject& project) : 
            Asset<IVertex, IProject>(project, project.createLiteralVertex(this, 0.0f, 0.0f, 0.0f)) {
  }

  IVertex* Vertex::createLiteralAsset(IProject& project) {
    return project.createLiteralVertex(this, 0.0f, 0.0f, 0.0f);
  }
  
  IVertex* Vertex::getAsset(IProject& project, JSONObject object) {
    return project.getVertex(this, object);
  }
  
  IVertex* Vertex::getAsset(IProject& project, const std::string& id) {
    return project.getVertex(this, id);
  }
  
  std::vector<std::string> Vertex::getAvailableProviders() const {
    return cManager.getAllVertices();
  }

  bool Vertex::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderVertexIcon(id);
  }

  bool Vertex::hasConfiguration() const {
    return cManager.isVertexConfigurable(getID());
  }

  bool Vertex::isDefaultConfiguration() const {
    return true;
  }
}
