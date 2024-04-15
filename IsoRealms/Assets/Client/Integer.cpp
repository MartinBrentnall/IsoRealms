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
#include "Integer.h"

namespace IsoRealms {
  Integer::Integer(IProject* project) :
            cProject(project),
            cInteger(cProject->createLiteralInteger(this)) {
  }

  void Integer::init(DOMNode& node, const std::string& tag) {
    DOMNode& mAssetNode = node.getNode(tag);
    cProject->init([this, &mAssetNode](IAssets* assets) {
      cProject->release(this, cInteger);
      cInteger = cProject->getInteger(this, mAssetNode);
    });
  }

  void Integer::save(DOMNodeWriter* node, const std::string& tag) {
    DOMNodeWriter mAssetNode = node->addBranch(tag);
    cProject->save(&mAssetNode, cInteger);
  }

  void Integer::relinquish(IInteger* asset) {
    if (cInteger == asset) {
      cInteger = cProject->createLiteralInteger(this);
    }
  }

  Integer::~Integer() {
    if (cInteger != nullptr) {
      cProject->release(this, cInteger);
    }
  }
}
