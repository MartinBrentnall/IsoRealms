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
#include "PhysicalObjectType.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  PhysicalObjectType::PhysicalObjectType(Spindizzy& spindizzy) :
            cSpindizzy(spindizzy),
            cPhysicalObjectType(cSpindizzy.createLiteralPhysicalObjectType(this)) {
  }

  void PhysicalObjectType::init(DOMNode& node) {
    cSpindizzy.getProject()->init([this, &node](IAssets* assets) {
      set(node);
    });
  }

  void PhysicalObjectType::save(DOMNodeWriter* node, const std::string& attribute) const {
    node->addAttribute(attribute, cSpindizzy.getID(cPhysicalObjectType));
  }

  std::string PhysicalObjectType::get() const {
    return cSpindizzy.getID(cPhysicalObjectType);
  }

  void PhysicalObjectType::set(DOMNode& node) {
    cSpindizzy.release(this, cPhysicalObjectType);
    cPhysicalObjectType = cSpindizzy.getPhysicalObjectType(this, node);
  }

  IBinding* PhysicalObjectType::getBinding(const std::string& id) const {
    return cPhysicalObjectType->getBinding(id);
  }

  void PhysicalObjectType::relinquish(IPhysicalObjectType* asset) {
    if (cPhysicalObjectType == asset) {
      cPhysicalObjectType = cSpindizzy.createLiteralPhysicalObjectType(this);
    }
  }

  PhysicalObjectType::~PhysicalObjectType() {
    if (cPhysicalObjectType != nullptr) {
      cSpindizzy.release(this, cPhysicalObjectType);
    }
  }
}

