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
#include "Editable.h"

namespace IsoRealms {
  Editable::Editable(IProject* project) :
            cProject(project),
            cEditable(cProject->createLiteralEditable(this)) {
  }

  void Editable::init(DOMNode& node) {
    cProject->init([this, &node](IAssets* assets) {
      set(node);
    });
  }

  void Editable::save(DOMNodeWriter* node, const std::string& attribute) {
    node->addAttribute(attribute, cProject->getID(cEditable));
  }

  std::string Editable::get() const {
    return cProject->getID(cEditable);
  }

  void Editable::set(DOMNode& node) {
    cProject->release(this, cEditable);
    cEditable = cProject->getEditable(this, node);
  }

  void Editable::relinquish(IEditable* asset) {
    if (cEditable == asset) {
      cEditable = cProject->createLiteralEditable(this);
    }
  }

  Editable::~Editable() {
    cProject->release(this, cEditable);
  }
}
