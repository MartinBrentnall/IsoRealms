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
#include "Screen.h"

namespace IsoRealms {
  Screen::Screen(IProject* project) :
            cProject(project),
            cScreen(cProject->createLiteralScreen(this)) {
  }

  void Screen::init(DOMNode& node) {
    cProject->init([this, &node](IAssets* assets) {
      set(node);
    });
  }

  void Screen::save(DOMNodeWriter* node, const std::string& tag) const {
    DOMNodeWriter mAssetNode = node->addBranch(tag);
    cProject->save(&mAssetNode, cScreen);
  }

  std::string Screen::get() const {
    return cProject->getID(cScreen);
  }

  void Screen::set(DOMNode& node) {
    cProject->release(this, cScreen);
    cScreen = cProject->getScreen(this, node);
  }

  void Screen::relinquish(IScreen* asset) {
    if (cScreen == asset) {
      cScreen = cProject->createLiteralScreen(this);
    }
  }

  Screen::~Screen() {
    if (cScreen != nullptr) {
      cProject->release(this, cScreen);
    }
  }
}

