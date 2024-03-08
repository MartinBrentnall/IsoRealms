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
#include "String.h"

namespace IsoRealms {
  String::String(IProject* project) :
            cProject(project),
            cString(cProject->createLiteralString(this)) {
  }

  void String::init(DOMNode& node) {
    cProject->init([this, &node](IAssets* assets) {
      set(node);
    });
  }

  void String::save(DOMNodeWriter* node, const std::string& attribute) const {
    node->addAttribute(attribute, cProject->getID(cString));
  }

  std::string String::get() const {
    return cProject->getID(cString);
  }

  void String::set(DOMNode& node) {
    cProject->release(this, cString);
    cString = cProject->getString(this, node);
  }

  void String::relinquish(IString* asset) {
    if (cString == asset) {
      cString = cProject->createLiteralString(this);
    }
  }

  String::~String() {
    if (cString != nullptr) {
      cProject->release(this, cString);
    }
  }
}

