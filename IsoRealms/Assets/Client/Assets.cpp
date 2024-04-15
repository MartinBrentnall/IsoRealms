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
#include "Assets.h"

namespace IsoRealms {
  Assets::Assets(IProject* project) :
            cProject(project),
            cAssets(cProject->createLiteralAssets(this)) {
  }

  void Assets::init(DOMNode& node, const std::string& tag) {
    DOMNode& mAssetNode = node.getNode(tag);
    cProject->init([this, &mAssetNode](IAssets* assets) {
      cProject->release(this, cAssets);
      cAssets = cProject->getAssets(this, mAssetNode);
    });
  }

  void Assets::save(DOMNodeWriter* node, const std::string& tag) const {
    DOMNodeWriter mAssetNode = node->addBranch(tag);
    cProject->save(&mAssetNode, cAssets);
  }

  void Assets::relinquish(IAssets* asset) {
    if (cAssets == asset) {
      cAssets = cProject->createLiteralAssets(this);
    }
  }

  Assets::~Assets() {
    if (cAssets != nullptr) {
      cProject->release(this, cAssets);
    }
  }
}
