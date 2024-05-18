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

  void WorldEditorTool::init(JSONObject object) {
    cSpindizzy->getProject()->init([this, object](IAssets* assets) {
      set(object);
    });
  }

  void WorldEditorTool::set(JSONObject object) {
    cSpindizzy->release(this, cWorldEditorTool);
    cWorldEditorTool = cSpindizzy->getWorldEditorTool(this, object);
  }

  void WorldEditorTool::save(JSONObject object, const std::string& name) const {
    JSONObject mAssetObject = object.addObject(name);
    cSpindizzy->save(mAssetObject, cWorldEditorTool);
  }

  void WorldEditorTool::relinquish(IWorldEditorTool* asset) {
    if (cWorldEditorTool == asset) {
      cWorldEditorTool = cSpindizzy->createLiteralWorldEditorTool(this);
    }
  }

  WorldEditorTool::~WorldEditorTool() {
    if (cWorldEditorTool != nullptr) {
      cSpindizzy->release(this, cWorldEditorTool);
    }
  }
}

