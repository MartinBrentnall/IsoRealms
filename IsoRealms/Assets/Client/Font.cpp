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
#include "Font.h"

namespace IsoRealms {
  Font::Font(IProject* project) :
            cProject(project),
            cFont(cProject->createLiteralFont(this)) {
  }

  void Font::init(DOMNode& node, const std::string& tag) {
    DOMNode& mAssetNode = node.getNode(tag);
    cProject->init([this, &mAssetNode](IAssets* assets) {
      set(mAssetNode);
    });
  }

  void Font::save(DOMNodeWriter* node, const std::string& tag) const {
    DOMNodeWriter mAssetNode = node->addBranch(tag);
    cProject->save(&mAssetNode, cFont);
  }

  void Font::set(DOMNode& node) {
    cProject->release(this, cFont);
    cFont = cProject->getFont(this, node);
  }

  void Font::relinquish(IFont* asset) {
    if (cFont == asset) {
      cFont = cProject->createLiteralFont(this);
    }
  }

  Font::~Font() {
    if (cFont != nullptr) {
      cProject->release(this, cFont);
    }
  }
}
