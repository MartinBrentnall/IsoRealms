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
#include "Editable.h"

namespace IsoRealms {
  Editable::Editable(IProject* project) :
            cProject(project),
            cEditable(cProject->createLiteralEditable(this)) {
  }

  void Editable::init(DOMNode& node, const std::string& tag) {
    cProject->init([this, &node, tag](IAssets* assets) {
      set(node, tag);
    });
  }

  void Editable::set(DOMNode& node, const std::string& tag) {
    DOMNode& mAssetNode = node.getNode(tag);
    cProject->release(this, cEditable);
    cEditable = cProject->getEditable(this, mAssetNode);
  }

  void Editable::save(DOMNodeWriter* node, const std::string& tag) {
    DOMNodeWriter mAssetNode = node->addBranch(tag);
    cProject->save(&mAssetNode, cEditable);
  }

  void Editable::relinquish(IEditable* asset) {
    if (cEditable == asset) {
      cEditable = cProject->createLiteralEditable(this);
    }
  }

  Editable::~Editable() {
    if (cEditable != nullptr) {
      cProject->release(this, cEditable);
    }
  }
}
