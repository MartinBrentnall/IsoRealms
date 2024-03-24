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
#include "Binding.h"

namespace IsoRealms {
  Binding::Binding(IProject* project, IBindingRegistry* registry) :
            cProject(project),
            cDefRegistry(registry),
            cDefBinding(cProject->createLiteralBinding(this)) {
  }

  void Binding::init(DOMNode& node) {
    cProject->init([this, &node](IAssets* assets) {
      set(node);
    });
  }

  void Binding::save(DOMNodeWriter* node, bool local, const std::string& tag) const {
    DOMNodeWriter mAssetNode = node->addBranch(tag);
    cProject->save(&mAssetNode, cDefBinding);
  }

//   std::string Binding::get() const {
//     return cDefRegistry->getBindingID(cDefBinding);
//   }

  void Binding::set(DOMNode& node) {
    cProject->release(this, cDefBinding);
    cDefBinding = cProject->getBinding(this, node, cDefRegistry);
  }

  void Binding::relinquish(IBinding* asset) {
    if (cDefBinding == asset) {
      cDefBinding = cProject->createLiteralBinding(this);
    }
  }

  Binding::~Binding() {
    if (cDefBinding != nullptr) {
      cProject->release(this, cDefBinding);
    }
  }
}
