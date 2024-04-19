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
#include "Boolean.h"

namespace IsoRealms {
  Boolean::Boolean(IProject* project, bool defaultValue, std::function<void(bool)> listener) :
            cProject(project),
            cDefaultValue(defaultValue),
            cBoolean(cProject->createLiteralBoolean(this, cDefaultValue)),
            cListener(listener) {
  }

  void Boolean::init(DOMNode& node, const std::string& tag) {
    cProject->init([this, &node, tag](IAssets* assets) {
      set(node, tag);
    });
  }

  void Boolean::set(DOMNode& node, const std::string& tag) {
    DOMNode& mAssetNode = node.getNode(tag);
    cProject->release(this, cBoolean);
    cBoolean = cProject->getBoolean(this, mAssetNode, cListener != nullptr ? this : nullptr);
  }

  void Boolean::save(DOMNodeWriter* node, const std::string& tag) const {
    DOMNodeWriter mAssetNode = node->addBranch(tag);
    cProject->save(&mAssetNode, cBoolean);
  }

  void Boolean::relinquish(IBoolean* asset) {
    if (cBoolean == asset) {
      cBoolean = cProject->createLiteralBoolean(this, cDefaultValue);
    }
  }

  void Boolean::stateChanged(IBoolean* boolean) {
    if (boolean == cBoolean) {
      cListener(cBoolean->getValue());
    }
  }

  Boolean::~Boolean() {
    if (cBoolean != nullptr) {
      cProject->release(this, cBoolean);
    }
  }
}

