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

  void Assets::init(DOMNode& node) {
    cProject->init([this, &node](IAssets* assets) {
      set(node);
    });
  }

  void Assets::save(DOMNodeWriter* node, const std::string& attribute) const {
    node->addAttribute(attribute, "TODO"); // TODO cProject->getID(cAssets));
  }

  std::string Assets::get() const {
    return "TODO";// TODO cProject->getID(cAssets);
  }

  void Assets::set(DOMNode& node) {
    cProject->release(this, cAssets);
    cAssets = cProject->getAssets(this, node);
  }

  void Assets::relinquish(IAssets* asset) {
    if (cAssets == asset) {
      cAssets = cProject->createLiteralAssets(this);
    }
  }

  Assets::~Assets() {
    cProject->release(this, cAssets);
  }
}
