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

namespace IsoRealms {
  Vertex::Vertex(IProject* project) :
            cProject(project),
            cVertex(cProject->createLiteralVertex(this, 0.0f, 0.0f, 0.0f)) {
  }

  void Vertex::init(DOMNode& node, const std::string& tag) {
    DOMNode& mAssetNode = node.getNode(tag);
    cProject->init([this, &mAssetNode](IAssets* assets) {
      set(mAssetNode);
    });
  }

  void Vertex::save(DOMNodeWriter* node, const std::string& tag) const {
    DOMNodeWriter mAssetNode = node->addBranch(tag);
    cProject->save(&mAssetNode, cVertex);
  }

  void Vertex::set(DOMNode& node) {
    cProject->release(this, cVertex);
    cVertex = cProject->getVertex(this, node);
  }

  void Vertex::relinquish(IVertex* asset) {
    if (cVertex == asset) {
      cVertex = cProject->createLiteralVertex(this, 0.0f, 0.0f, 0.0f);
    }
  }

  Vertex::~Vertex() {
    if (cVertex != nullptr) {
      cProject->release(this, cVertex);
    }
  }
}
