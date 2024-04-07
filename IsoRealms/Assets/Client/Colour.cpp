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
#include "Colour.h"

namespace IsoRealms {
  Colour::Colour(IProject* project, float defaultRed, float defaultGreen, float defaultBlue, float defaultAlpha, std::function<void()> listener) :
            cProject(project),
            cDefaultRed(defaultRed),
            cDefaultGreen(defaultGreen),
            cDefaultBlue(defaultBlue),
            cDefaultAlpha(defaultAlpha),
            cColour(cProject->createLiteralColour(this, cDefaultRed, cDefaultGreen, cDefaultBlue, cDefaultAlpha)),
            cListener(listener) {
  }

  void Colour::init(DOMNode& node, const std::string& tag) {
    DOMNode& mAssetNode = node.getNode(tag);
    cProject->init([this, &mAssetNode](IAssets* assets) {
      set(mAssetNode);
    });
  }

  void Colour::save(DOMNodeWriter* node, const std::string& tag) const {
    DOMNodeWriter mAssetNode = node->addBranch(tag);
    cProject->save(&mAssetNode, cColour);
  }

  void Colour::set(DOMNode& node) {
    cProject->release(this, cColour);
    cColour = cProject->getColour(this, node, cListener != nullptr ? this : nullptr);
  }

  void Colour::relinquish(IColour* asset) {
    if (cColour == asset) {
      cColour = cProject->createLiteralColour(this, cDefaultRed, cDefaultGreen, cDefaultBlue, cDefaultAlpha);
    }
  }

  void Colour::stateChanged(IColour* colour) {
    if (colour == cColour) {
      cListener();
    }
  }

  Colour::~Colour() {
    if (cColour != nullptr) {
      cProject->release(this, cColour);
    }
  }
}
