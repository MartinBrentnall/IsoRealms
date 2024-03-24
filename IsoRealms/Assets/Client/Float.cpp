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
#include "Float.h"

namespace IsoRealms {
  Float::Float(IProject* project, float defaultValue, std::function<void(float)> listener) :
            cProject(project),
            cDefaultValue(defaultValue),
            cFloat(cProject->createLiteralFloat(this, cDefaultValue)),
            cListener(listener) {
  }

  void Float::init(DOMNode& node) {
    cProject->init([this, &node](IAssets* assets) {
      set(node);
    });
  }

  void Float::save(DOMNodeWriter* node, const std::string& tag) const {
    DOMNodeWriter mAssetNode = node->addBranch(tag);
    cProject->save(&mAssetNode, cFloat);
  }

  std::string Float::get() const {
    return cProject->getID(cFloat);
  }

  void Float::set(DOMNode& node) {
    cProject->release(this, cFloat);
    cFloat = cProject->getFloat(this, node, cListener != nullptr ? this : nullptr);
  }

  void Float::relinquish(IFloat* asset) {
    if (cFloat == asset) {
      cFloat = cProject->createLiteralFloat(this, cDefaultValue);
    }
  }

  void Float::stateChanged(IFloat* value) {
    if (value == cFloat) {
      cListener(cFloat->getValue());
    }
  }

  Float::~Float() {
    if (cFloat != nullptr) {
      cProject->release(this, cFloat);
    }
  }
}

