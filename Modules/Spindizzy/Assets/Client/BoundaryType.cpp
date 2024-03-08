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
#include "BoundaryType.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  BoundaryType::BoundaryType(Spindizzy& spindizzy) :
            cSpindizzy(spindizzy),
            cBoundaryType(cSpindizzy.createLiteralBoundaryType(this)) {
  }

  void BoundaryType::init(DOMNode& node) {
    cSpindizzy.getProject()->init([this, &node](IAssets* assets) {
      set(node);
    });
  }

  void BoundaryType::save(DOMNodeWriter* node, const std::string& attribute) const {
    node->addAttribute(attribute, cSpindizzy.getID(cBoundaryType));
  }

  std::string BoundaryType::get() const {
    return cSpindizzy.getID(cBoundaryType);
  }

  void BoundaryType::set(DOMNode& node) {
    cSpindizzy.release(this, cBoundaryType);
    cBoundaryType = cSpindizzy.getBoundaryType(this, node);
  }

  void BoundaryType::relinquish(IBoundaryType* asset) {
    if (cBoundaryType == asset) {
      cBoundaryType = cSpindizzy.createLiteralBoundaryType(this);
    }
  }

  IBinding* BoundaryType::getBoundaryBinding(const std::string& id) const {
    return cBoundaryType->getBinding(id);
  }

  BoundaryType::~BoundaryType() {
    if (cBoundaryType != nullptr) {
      cSpindizzy.release(this, cBoundaryType);
    }
  }
}

