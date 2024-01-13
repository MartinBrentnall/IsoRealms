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
#include "WorldEditorTool.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  WorldEditorTool::WorldEditorTool(Spindizzy* spindizzy) :
            cSpindizzy(spindizzy),
            cWorldEditorTool(cSpindizzy->createLiteralWorldEditorTool(this)) {
  }

  void WorldEditorTool::init(DOMNode& node) {
    cSpindizzy->getProject()->init([this, &node](IAssets* assets) {
      set(node);
    });
  }

  void WorldEditorTool::save(DOMNodeWriter* node, const std::string& attribute) const {
    // TODO: Implement this.
  }

  std::string WorldEditorTool::get() const {
    return cSpindizzy->getID(cWorldEditorTool);
  }

  void WorldEditorTool::set(DOMNode& node) {
    cSpindizzy->release(this, cWorldEditorTool);
    cWorldEditorTool = cSpindizzy->getWorldEditorTool(this, node);
  }

  void WorldEditorTool::relinquish(IWorldEditorTool* asset) {
    if (cWorldEditorTool == asset) {
      cWorldEditorTool = cSpindizzy->createLiteralWorldEditorTool(this);
    }
  }

  WorldEditorTool::~WorldEditorTool() {
    cSpindizzy->release(this, cWorldEditorTool);
  }
}

