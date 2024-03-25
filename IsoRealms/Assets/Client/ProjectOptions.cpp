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
#include "ProjectOptions.h"

namespace IsoRealms {
  ProjectOptions::ProjectOptions(IProject* project) :
            cProject(project),
            cProjectOptions(cProject->createLiteralProjectOptions(this)) {
  }

  void ProjectOptions::init(DOMNode& node) {
    cProject->init([this, &node](IAssets* assets) {
      set(node);
    });
  }

  void ProjectOptions::save(DOMNodeWriter* node, const std::string& tag) const {
    DOMNodeWriter mAssetNode = node->addBranch(tag);
    cProject->save(&mAssetNode, cProjectOptions);
  }

  void ProjectOptions::set(DOMNode& node) {
    cProject->release(this, cProjectOptions);
    cProjectOptions = cProject->getProjectOptions(this, node);
  }

  void ProjectOptions::relinquish(IProjectOptions* asset) {
    if (cProjectOptions == asset) {
      cProjectOptions = cProject->createLiteralProjectOptions(this);
    }
  }

  ProjectOptions::~ProjectOptions() {
    if (cProjectOptions != nullptr) {
      cProject->release(this, cProjectOptions);
    }
  }
}
